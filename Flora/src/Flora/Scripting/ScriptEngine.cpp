#include "flpch.h"
#include "ScriptEngine.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"

namespace Flora {
	struct ScriptEngineData {
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;
		MonoAssembly* CoreAssembly = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init() {
		s_Data = new ScriptEngineData();
		InitMono();
	}

	void ScriptEngine::Shutdown() {
		ShutdownMono();
		delete s_Data;
	}

	char* ReadBytes(const std::string& filepath, uint32_t* outSize) {
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);
		if (!stream) {
			FL_CORE_ERROR("Failed to open file!");
			return nullptr;
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();
		if (size == 0) {
			FL_CORE_ERROR("File is empty");
			return nullptr;
		}

		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
	}

	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath) {
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);
		
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);
		if (status != MONO_IMAGE_OK) {
			const char* errorMessage = mono_image_strerror(status);
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		delete[] fileData;
		return assembly;
	}

	void PrintAssemblyTypes(MonoAssembly* assembly) {
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

	void ScriptEngine::InitMono() {
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("FloraJITRuntime");
		FL_CORE_ASSERT(rootDomain, "Root domain failed to initialize!");
		s_Data->RootDomain = rootDomain;
		s_Data->AppDomain = mono_domain_create_appdomain("FloraScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = LoadCSharpAssembly("resources/Scripts/Flora-ScriptCore.dll");
		PrintAssemblyTypes(s_Data->CoreAssembly);

		// testing
		{
			MonoImage* assemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);

			MonoClass* monoClass = mono_class_from_name(assemblyImage, "Flora", "Main");
			MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
			mono_runtime_object_init(instance);

			MonoMethod* printMessageFunc = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
			mono_runtime_invoke(printMessageFunc, instance, nullptr, nullptr);

			MonoMethod* printIntFunc = mono_class_get_method_from_name(monoClass, "PrintInt", 1);
			MonoMethod* printCustomFunc = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
			int value = 5;
			void* param = &value;
			mono_runtime_invoke(printIntFunc, instance, &param, nullptr);
			MonoString* monostring = mono_string_new(s_Data->AppDomain, "Hello World From C++");
			void* stringParam = monostring;
			mono_runtime_invoke(printCustomFunc, instance, &stringParam, nullptr);
		}
	}

	void ScriptEngine::ShutdownMono() {
		//mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
		//mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}
}