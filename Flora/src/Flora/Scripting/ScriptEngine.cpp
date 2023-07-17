#include "flpch.h"
#include "ScriptEngine.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/tabledefs.h"
#include <glm/glm.hpp>
#include "ScriptGlue.h"
#include "Flora/Scene/Components.h"
#include "FileWatch.h"
#include "Flora/Core/Application.h"
#include "Flora/Core/Timer.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
#include "Flora/Core/Buffer.h"
#include "Flora/Core/FileSystem.h"
#include "Flora/Project/Project.h"

namespace Flora {

	static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap = {
		{ "System.Single", ScriptFieldType::Float },
		{ "System.Double", ScriptFieldType::Double }
	};

	namespace Utils {
		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath, bool loadPDB = false) {
			ScopedBuffer fileData = FileSystem::ReadFileBinary(assemblyPath);

			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData.As<char>(), fileData.Size(), 1, &status, 0);
			if (status != MONO_IMAGE_OK) {
				const char* errorMessage = mono_image_strerror(status);
				return nullptr;
			}

			if (loadPDB) {
				std::filesystem::path pdbPath = assemblyPath;
				pdbPath.replace_extension(".pdb");

				if (std::filesystem::exists(pdbPath)) {
					uint32_t pdbFileSize = 0;
					ScopedBuffer pdbFileData = FileSystem::ReadFileBinary(pdbPath);
					mono_debug_open_image_from_memory(image, pdbFileData.As<const mono_byte>(), pdbFileData.Size());
					FL_CORE_INFO("Loaded PDB {}", pdbPath);
				}
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
			mono_image_close(image);
			return assembly;
		}

		static void PrintAssemblyTypes(MonoAssembly* assembly) {
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++) {
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);
				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				FL_CORE_TRACE("{}.{}", nameSpace, name);
			}
		}

		ScriptFieldType MonoTypeToScriptType(MonoType* type) {
			std::string typeName = mono_type_get_name(type);
			auto it = s_ScriptFieldTypeMap.find(typeName);
			if(it == s_ScriptFieldTypeMap.end())
				return ScriptFieldType::None;
			return it->second;
		}
	}

	struct ScriptEngineData {
		MonoDomain* RootDomain       = nullptr;
		MonoDomain* AppDomain        = nullptr;
		MonoAssembly* CoreAssembly   = nullptr;
		MonoAssembly* AppAssembly    = nullptr;
		MonoImage* AppAssemblyImage  = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;
		Scene* SceneContext          = nullptr;

		std::filesystem::path CoreAssemblyFilepath;
		std::filesystem::path AppAssemblyFilepath;

		ScriptClass EntityClass;
		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<uint32_t, Ref<ScriptInstance>> EntityInstances;
		std::unordered_map<uint32_t, ScriptFieldMap> EntityScriptFields;

		Scope<filewatch::FileWatch<std::string>> AppAssemblyFileWatcher;
		bool AssemblyReloadPending = false;
		bool EnableDebugging = false;
	};

	static ScriptEngineData* s_Data = nullptr;

	static void OnAppAssemblyFileSystemEvent(const std::string& path, const filewatch::Event change_type) {
		if (!s_Data->AssemblyReloadPending && change_type == filewatch::Event::modified) {
			s_Data->AssemblyReloadPending = true;
			Application::Get().SubmitToMainThread([]() {
				s_Data->AppAssemblyFileWatcher.reset();
				ScriptEngine::ReloadAssembly();
			});
		}
	}

	void ScriptEngine::Init() {
		s_Data = new ScriptEngineData();
		InitMono();
		ScriptGlue::RegisterFunctions();
		bool status = LoadAssembly("Resources/Scripts/Flora-ScriptCore.dll");
		if (!status) {
			FL_CORE_ERROR("[ScriptEngine] Could not load Flora-ScriptCore assembly.");
			return;
		}
		
		status = LoadAppAssembly("Sandbox Project/Assets/Scripts/Binaries/" + Project::GetActive()->GetConfig().Name + ".dll");
		if (!status) {
			FL_CORE_ERROR("[ScriptEngine] Could not load app assembly.");
			return;
		}
		LoadAssemblyClasses();
		ScriptGlue::RegisterComponents();

		s_Data->EntityClass = ScriptClass("Flora", "Entity", true);
		s_Data->EntityClass.Instantiate();
		m_Initialized = true;
	}

	void ScriptEngine::Shutdown() {
		ShutdownMono();
		delete s_Data;
		m_Initialized = false;
	}

	bool ScriptEngine::LoadAssembly(const std::filesystem::path& filepath) {
		s_Data->AppDomain = mono_domain_create_appdomain("FloraScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath, s_Data->EnableDebugging);
		if (s_Data->CoreAssembly == nullptr)
			return false;
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		s_Data->CoreAssemblyFilepath = filepath;
		return true;
	}

	bool ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath) {
		s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath, s_Data->EnableDebugging);
		if (s_Data->AppAssembly == nullptr)
			return false;
		s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->AppAssembly);
		s_Data->AppAssemblyFilepath = filepath;
		s_Data->AppAssemblyFileWatcher = CreateScope<filewatch::FileWatch<std::string>>(filepath.string(), OnAppAssemblyFileSystemEvent);
		s_Data->AssemblyReloadPending = false;
		return true;
	}

	void ScriptEngine::ReloadAssembly() {
		mono_domain_set(mono_get_root_domain(), false);
		mono_domain_unload(s_Data->AppDomain);
		LoadAssembly(s_Data->CoreAssemblyFilepath);
		LoadAppAssembly(s_Data->AppAssemblyFilepath);
		LoadAssemblyClasses();
		ScriptGlue::RegisterComponents();
		s_Data->EntityClass = ScriptClass("Flora", "Entity", true);
	}

	void ScriptEngine::InitMono() {
		mono_set_assemblies_path("mono/lib");
		if (s_Data->EnableDebugging){
			const char* argv[2] = {
				"--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,suspend=n,loglevel=3,logfile=MonoDebugger.log",
				"--soft-breakpoints"
			};
			mono_jit_parse_options(2, (char**)argv);
			mono_debug_init(MONO_DEBUG_FORMAT_MONO);
		}
		MonoDomain* rootDomain = mono_jit_init("FloraJITRuntime");
		FL_CORE_ASSERT(rootDomain, "Root domain failed to initialize!");
		s_Data->RootDomain = rootDomain;
		if (s_Data->EnableDebugging)
			mono_debug_domain_create(s_Data->RootDomain);
		mono_thread_set_main(mono_thread_current());
	}

	void ScriptEngine::ShutdownMono() {
		mono_domain_set(mono_get_root_domain(), false);
		mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass) {
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance);
		return instance;
	}

	std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetEntityClasses() {
		return s_Data->EntityClasses;
	}

	MonoImage* ScriptEngine::GetCoreAssemblyImage() {
		return s_Data->CoreAssemblyImage;
	}

	void ScriptEngine::LoadAssemblyClasses() {
		s_Data->EntityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Flora", "Entity");

		for (int32_t i = 0; i < numTypes; i++) {
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);
			const char* nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, className);
			else
				fullName = className;

			MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, className);
			if (monoClass == entityClass) continue;
			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
			if (!isEntity)
				continue;

			Ref<ScriptClass> scriptClass = CreateRef<ScriptClass>(nameSpace, className);
			s_Data->EntityClasses[fullName] = scriptClass;
			mono_class_num_fields(monoClass);
			void* iterator = nullptr;
			while (MonoClassField* field = mono_class_get_fields(monoClass, &iterator)) {
				const char* fieldName = mono_field_get_name(field);
				uint32_t flags = mono_field_get_flags(field);
				if (flags & FIELD_ATTRIBUTE_PUBLIC) {
					MonoType* type = mono_field_get_type(field);
					ScriptFieldType fieldType = Utils::MonoTypeToScriptType(type);
					scriptClass->m_Fields[fieldName] = { fieldType, fieldName, field };
				}
			}
		}
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene) {
		s_Data->SceneContext = scene;
	}

	void ScriptEngine::OnRuntimeStop() {
		s_Data->SceneContext = nullptr;
		s_Data->EntityInstances.clear();
	}

	void ScriptEngine::CreateEntity(Entity entity) {
		const auto& sc = entity.GetComponent<ScriptComponent>();
		if (ScriptEngine::EntityClassExists(sc.ClassName)) {
			Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_Data->EntityClasses[sc.ClassName], entity);
			s_Data->EntityInstances[(uint32_t)entity] = instance;

			// Copy field values
			if (s_Data->EntityScriptFields.find((uint32_t)entity) != s_Data->EntityScriptFields.end()) {
				const ScriptFieldMap& fieldMap = s_Data->EntityScriptFields.at((uint32_t)entity);
				for (const auto& [name, fieldInstance] : fieldMap)
					instance->SetFieldValueInternal(name, fieldInstance.m_DataBuffer);
			}

			instance->InvokeOnCreate();
		}
	}

	void ScriptEngine::UpdateEntity(Entity entity, float ts) {
		if (s_Data->EntityInstances.find((uint32_t)entity) != s_Data->EntityInstances.end()) {
			Ref<ScriptInstance> instance = s_Data->EntityInstances[(uint32_t)entity];
			instance->InvokeOnUpdate((float)ts);
		} else {
			FL_CORE_ERROR("Could not find ScriptInstance for entity {}", (uint32_t)entity);
		}
	}

	void ScriptEngine::DestroyEntity(Entity entity) {
		FL_CORE_ASSERT(s_Data->EntityInstances.find((uint32_t)entity) != s_Data->EntityInstances.end(), "Entity script object not found");
		s_Data->EntityInstances[(uint32_t)entity]->InvokeOnDestroy();
	}

	Ref<ScriptClass> ScriptEngine::GetEntityClass(const std::string& name) {
		if (s_Data->EntityClasses.find(name) == s_Data->EntityClasses.end())
			return nullptr;
		return s_Data->EntityClasses.at(name);
	}

	Scene* ScriptEngine::GetSceneContext() {
		return s_Data->SceneContext;
	}

	Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(Entity entity) {
		auto it = s_Data->EntityInstances.find((uint32_t)entity);
		if (it == s_Data->EntityInstances.end())
			return nullptr;
		return it->second;
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullName) {
		return s_Data->EntityClasses.find(fullName) != s_Data->EntityClasses.end();
	}

	MonoObject* ScriptEngine::GetManagedInstance(uint32_t eid) {
		FL_CORE_ASSERT(s_Data->EntityInstances.find(entity) != s_Data->EntityInstances.end());
		return s_Data->EntityInstances.at(eid)->GetManagedObject();
	}

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore)
		: m_Namespace(classNamespace), m_ClassName(className) {
		m_MonoClass = mono_class_from_name(isCore ? s_Data->CoreAssemblyImage : s_Data->AppAssemblyImage, classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate() {
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& methodName, int parameterCount) {
		return mono_class_get_method_from_name(m_MonoClass, methodName.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params) {
		MonoObject* exception = nullptr;
		return mono_runtime_invoke(method, instance, params, &exception);
	}

	ScriptFieldMap& ScriptEngine::GetScriptFieldMap(Entity entity) {
		FL_CORE_ASSERT(entity);
		//FL_CORE_ASSERT(s_Data->EntityScriptFields.find((uint32_t)entity) != s_Data->EntityScriptFields.end());
		return s_Data->EntityScriptFields[(uint32_t)entity];
	}

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity)
		: m_ScriptClass(scriptClass) {
		m_Instance = m_ScriptClass->Instantiate();
		m_Constructor = s_Data->EntityClass.GetMethod(".ctor", 1);
		m_OnCreateMethod = m_ScriptClass->GetMethod("OnCreate", 0);
		m_OnDestroyMethod = m_ScriptClass->GetMethod("OnDestroy", 0);
		m_OnUpdateMethod = m_ScriptClass->GetMethod("OnUpdate", 1);

		// Call entity constructor here
		uint32_t eid = (uint32_t)entity;
		void* param = &eid;
		m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &param);
	}

	void ScriptInstance::InvokeOnCreate() {
		if (m_OnCreateMethod)
			m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod);
		else
			FL_CORE_WARN("Create method is missing");
	}

	void ScriptInstance::InvokeOnDestroy() {
		if (m_OnCreateMethod)
			m_ScriptClass->InvokeMethod(m_Instance, m_OnDestroyMethod);
		else
			FL_CORE_WARN("Destroy method is missing");
	}

	void ScriptInstance::InvokeOnUpdate(float ts) {
		if (m_OnUpdateMethod) {
			void* param = &ts;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, &param);
		} else
			FL_CORE_WARN("Update method is missing");
	}

	bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer) {
		const auto& fields = m_ScriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;
		const ScriptField& field = it->second;
		mono_field_get_value(m_Instance, field.ClassField, buffer);
		return true;
	}

	bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value) {
		const auto& fields = m_ScriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;
		const ScriptField& field = it->second;
		mono_field_set_value(m_Instance, field.ClassField, (void*)value);
		return true;
	}
}