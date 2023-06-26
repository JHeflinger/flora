#include "flpch.h"
#include "ScriptGlue.h"
#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"
#include "Flora/Scripting/ScriptEngine.h"
#include "Flora/Core/Input.h"
#include <glm/glm.hpp>

namespace Flora {
	
	#define FL_ADD_INTERNAL_CALL(funcName) mono_add_internal_call("Flora.InternalCalls::" #funcName, funcName)

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_HasComponentFunctions;

	static void CoreTrace(MonoString* monostring) {
		char* string = mono_string_to_utf8(monostring);
		FL_CORE_TRACE(string);
		mono_free(string);
	}

	static void TransformComponent_GetTranslation(uint32_t eid, glm::vec3* outTranslation) {
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityFromID(eid);
		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(uint32_t eid, glm::vec3* inTranslation) {
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityFromID(eid);
		entity.GetComponent<TransformComponent>().Translation = *inTranslation;
	}

	static bool Input_IsKeyDown(KeyCode keycode) {
		return Input::IsKeyPressed(keycode);
	}

	static bool Entity_HasComponent(uint32_t eid, MonoReflectionType* componentType) {
		Scene* scene = ScriptEngine::GetSceneContext();
		FL_CORE_ASSERT(scene != nullptr);
		Entity entity = scene->GetEntityFromID(eid);
		FL_CORE_ASSERT(entity != nullptr);
		MonoType* monoComponentType = mono_reflection_type_get_type(componentType);
		FL_CORE_ASSERT(s_HasComponentFunctions.find(monoComponentType) != s_HasComponentFunctions.end());
		return s_HasComponentFunctions.at(monoComponentType)(entity);
	}

	void ScriptGlue::RegisterFunctions() {
		FL_ADD_INTERNAL_CALL(CoreTrace);
		FL_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		FL_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		FL_ADD_INTERNAL_CALL(Input_IsKeyDown);
		FL_ADD_INTERNAL_CALL(Entity_HasComponent);
	}

	template<typename... Component>
	void ScriptGlue::RegisterComponent() {
		([]() {
			std::string_view typeName = typeid(Component).name();
			std::string_view structName = typeName.substr(typeName.find_last_of(':') + 1);
			std::string formattedName = fmt::format("Flora.{}", structName);
			MonoType* managedType = mono_reflection_type_from_name(formattedName.data(), ScriptEngine::GetCoreAssemblyImage());
			if (!managedType) {
				FL_CORE_WARN("Could not find component type {}", formattedName);
				return;
			}
			s_HasComponentFunctions[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
		}(), ...);
	}

	template<typename... Component>
	static void ScriptGlue::RegisterComponent(ComponentGroup<Component ...>) {
		RegisterComponent<Component ...>();
	}

	void ScriptGlue::RegisterComponents() {
		RegisterComponent(AllComponents{});
	}
}