#include "flpch.h"
#include "ScriptEngine.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include <glm/glm.hpp>
#include "ScriptGlue.h"
#include "Flora/Scene/Components.h"

namespace Flora {
	namespace Utils {
		//TODO: move to general utils file
		static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize) {
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);
			if (!stream) {
				FL_CORE_ERROR("Failed to open file!");
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint64_t size = end - stream.tellg();
			if (size == 0) {
				FL_CORE_ERROR("File is empty");
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = (uint32_t)size;
			return buffer;
		}

		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath) {
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);
			if (status != MONO_IMAGE_OK) {
				const char* errorMessage = mono_image_strerror(status);
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
			delete[] fileData;
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
	}

	struct ScriptEngineData {
		MonoDomain* RootDomain       = nullptr;
		MonoDomain* AppDomain        = nullptr;
		MonoAssembly* CoreAssembly   = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;
		Scene* SceneContext          = nullptr;

		ScriptClass EntityClass;
		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<uint32_t, Ref<ScriptInstance>> EntityInstances;
	};

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init() {
		s_Data = new ScriptEngineData();
		InitMono();
		LoadAssembly("resources/Scripts/Flora-ScriptCore.dll");

		ScriptGlue::RegisterFunctions();

		s_Data->EntityClass = ScriptClass("Flora", "Entity");
		s_Data->EntityClass.Instantiate();
		LoadAssemblyClasses();
	}

	void ScriptEngine::Shutdown() {
		ShutdownMono();
		delete s_Data;
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath) {
		s_Data->AppDomain = mono_domain_create_appdomain("FloraScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath);
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
	}

	void ScriptEngine::InitMono() {
		mono_set_assemblies_path("mono/lib");
		MonoDomain* rootDomain = mono_jit_init("FloraJITRuntime");
		FL_CORE_ASSERT(rootDomain, "Root domain failed to initialize!");
		s_Data->RootDomain = rootDomain;
	}

	void ScriptEngine::ShutdownMono() {
		//mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
		//mono_jit_cleanup(s_Data->RootDomain);
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

	void ScriptEngine::LoadAssemblyClasses() {
		LoadAssemblyClasses(s_Data->CoreAssembly);
	}

	void ScriptEngine::LoadAssemblyClasses(MonoAssembly* assembly) {
		s_Data->EntityClasses.clear();

		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
		MonoClass* entityClass = mono_class_from_name(image, "Flora", "Entity");

		for (int32_t i = 0; i < numTypes; i++) {
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);
			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, name);
			else
				fullName = name;

			MonoClass* monoClass = mono_class_from_name(image, nameSpace, name);
			if (monoClass == entityClass) continue;
			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
			if (isEntity)
				s_Data->EntityClasses[fullName] = CreateRef<ScriptClass>(nameSpace, name);
		}
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene) {
		s_Data->SceneContext = scene;
	}

	void ScriptEngine::OnRuntimeStop() {
		s_Data->SceneContext = nullptr;
	}

	void ScriptEngine::CreateEntity(Entity entity) {
		const auto& sc = entity.GetComponent<ScriptComponent>();
		if (ScriptEngine::EntityClassExists(sc.ClassName)) {
			Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_Data->EntityClasses[sc.ClassName]);
			s_Data->EntityInstances[(uint32_t)entity] = instance;
			instance->InvokeOnCreate();
		}
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullName) {
		return s_Data->EntityClasses.find(fullName) != s_Data->EntityClasses.end();
	}

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className)
		: m_Namespace(classNamespace), m_ClassName(className) {
		m_MonoClass = mono_class_from_name(s_Data->CoreAssemblyImage, classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate() {
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& methodName, int parameterCount) {
		return mono_class_get_method_from_name(m_MonoClass, methodName.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params) {
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass)
		: m_ScriptClass(scriptClass) {
		m_Instance = m_ScriptClass->Instantiate();
		m_OnCreateMethod = m_ScriptClass->GetMethod("OnCreate", 0);
		m_OnDestroyMethod = m_ScriptClass->GetMethod("OnDestroy", 0);
		m_OnUpdateMethod = m_ScriptClass->GetMethod("OnUpdate", 1);
	}

	void ScriptInstance::InvokeOnCreate() {
		m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod);
	}

	void ScriptInstance::InvokeOnDestroy() {
		m_ScriptClass->InvokeMethod(m_Instance, m_OnDestroyMethod);
	}

	void ScriptInstance::InvokeOnUpdate(float ts) {
		void* param = &ts;
		m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, &param);
	}
}