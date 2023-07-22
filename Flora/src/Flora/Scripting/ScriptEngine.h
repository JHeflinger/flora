#pragma once
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"
#include <filesystem>
#include <string>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}

namespace Flora {

	enum class ScriptFieldType {
		None = 0,
		Float,
		Vector2,
		Vector3,
		Vector4,
		Int,
		UInt,
		Bool,
		Double,
		Short,
		Byte,
		String,
		Entity
	};

	struct ScriptField {
		ScriptFieldType Type;
		std::string Name;
		MonoClassField* ClassField;
	};

	struct ScriptFieldInstance {
		ScriptField Field;

		ScriptFieldInstance() {
			memset(m_DataBuffer, 0, sizeof(m_DataBuffer));
		}

		template<typename T>
		T GetValue() {
			return *(T*)m_DataBuffer;
		}

		template<typename T>
		void SetValue(T value) {
			FL_CORE_ASSERT(sizeof(T) <= 8, "type too large!");
			memcpy(m_DataBuffer, &value, sizeof(T));
		}
	private:
		uint8_t m_DataBuffer[8];
		friend class ScriptInstance;
		friend class ScriptEngine;
	};

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

	class ScriptClass {
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);
		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& methodName, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);
		const std::map<std::string, ScriptField>& GetFields() const { return m_Fields; }
	private:
		std::map<std::string, ScriptField> m_Fields;
		std::string m_Namespace, m_ClassName;
		MonoClass* m_MonoClass = nullptr;
	private:
		friend class ScriptEngine;
	};

	class ScriptInstance {
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);
		void InvokeOnCreate();
		void InvokeOnDestroy();
		void InvokeOnUpdate(float ts);
		Ref<ScriptClass> GetScriptClass() { return m_ScriptClass; }
		MonoObject* GetManagedObject() { return m_Instance; }
	public:
		template<typename T>
		T GetFieldValue(const std::string& name) {
			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if (!success) return T();
			return *(T*)s_FieldValueBuffer;
		}
		
		template<typename T>
		void SetFieldValue(const std::string& name, const T& value) {
			bool success = SetFieldValueInternal(name, &value);
		}
	private:
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);
	private:
		Ref<ScriptClass> m_ScriptClass;
		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnDestroyMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;
		inline static char s_FieldValueBuffer[8];
		friend struct ScriptFieldInstance;
		friend class ScriptEngine;
	};

	class ScriptEngine {
	public:
		static void Init();
		static void Shutdown();
		static bool LoadAssembly(const std::filesystem::path& filepath);
		static bool LoadAppAssembly(const std::filesystem::path& filepath);
		static void ReloadAssembly();
		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();
		static bool EntityClassExists(const std::string& fullName);
		static void CreateEntity(Entity entity);
		static void UpdateEntity(Entity entity, float ts);
		static void DestroyEntity(Entity entity);
		static Scene* GetSceneContext();
		static Ref<ScriptInstance> GetEntityScriptInstance(Entity entity);
		static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();
		static ScriptFieldMap& GetScriptFieldMap(Entity entity);
		static Ref<ScriptClass> GetEntityClass(const std::string& name);
		static MonoObject* GetManagedInstance(uint32_t eid);
		inline static bool IsInitialized() { return m_Initialized; }
	private:
		static void InitMono();
		static void ShutdownMono();
		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();
		static MonoImage* GetCoreAssemblyImage();
	private:
		inline static bool m_Initialized = false;
		friend class ScriptClass;
		friend class ScriptGlue;
	};

	namespace Utils {
		inline const char* ScriptFieldTypeToString(ScriptFieldType type) {
			switch (type) {
			case ScriptFieldType::Float:
				return "Float";
			case ScriptFieldType::Vector2:
				return "Vector2";
			case ScriptFieldType::Vector3:
				return "Vector3";
			case ScriptFieldType::Vector4:
				return "Vector4";
			case ScriptFieldType::Int:
				return "Int";
			case ScriptFieldType::UInt:
				return "UInt";
			case ScriptFieldType::Bool:
				return "Bool";
			case ScriptFieldType::Double:
				return "Double";
			case ScriptFieldType::Short:
				return "Short";
			case ScriptFieldType::Byte:
				return "Byte";
			case ScriptFieldType::String:
				return "String";
			case ScriptFieldType::Entity:
				return "Entity";
			}
			FL_CORE_ASSERT(false, "Unknown field type!");
			return "None";
		}

		inline ScriptFieldType ScriptFieldTypeFromSting(std::string_view type) {
			if (type == "Float") return ScriptFieldType::Float;
			if (type == "Vector2") return ScriptFieldType::Vector2;
			if (type == "Vector3") return ScriptFieldType::Vector3;
			if (type == "Vector4") return ScriptFieldType::Vector4;
			if (type == "Int") return ScriptFieldType::Int;
			if (type == "UInt") return ScriptFieldType::UInt;
			if (type == "Bool") return ScriptFieldType::Bool;
			if (type == "Double") return ScriptFieldType::Double;
			if (type == "Short") return ScriptFieldType::Short;
			if (type == "Byte") return ScriptFieldType::Byte;
			if (type == "String") return ScriptFieldType::String;
			if (type == "Entity") return ScriptFieldType::Entity;
			FL_CORE_ASSERT(false, "Unknown field type!");
			return ScriptFieldType::None;
		}
	}
}