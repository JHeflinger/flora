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
}

namespace Flora {
	class ScriptClass {
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);
		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& methodName, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);
	private:
		std::string m_Namespace, m_ClassName;
		MonoClass* m_MonoClass = nullptr;
	};

	class ScriptInstance {
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);
		void InvokeOnCreate();
		void InvokeOnDestroy();
		void InvokeOnUpdate(float ts);
	private:
		Ref<ScriptClass> m_ScriptClass;
		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnDestroyMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;
	};

	class ScriptEngine {
	public:
		static void Init();
		static void Shutdown();
		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);
		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();
		static bool EntityClassExists(const std::string& fullName);
		static void CreateEntity(Entity entity);
		static void UpdateEntity(Entity entity, float ts);
		static void DestroyEntity(Entity entity);
		static Scene* GetSceneContext();
		static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();
	private:
		static void InitMono();
		static void ShutdownMono();
		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();
		static MonoImage* GetCoreAssemblyImage();
	private:
		friend class ScriptClass;
		friend class ScriptGlue;
	};
}