#include "flpch.h"
#include "ScriptGlue.h"
#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"
#include "Flora/Scripting/ScriptEngine.h"
#include "Flora/Core/Input.h"
#include "Flora/Utils/PhysicsUtils.h"
#include <glm/glm.hpp>
#include "mono/jit/jit.h"

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
	
	static void TransformComponent_GetRotation(uint32_t eid, glm::vec3* outRotation) {
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityFromID(eid);
		*outRotation = entity.GetComponent<TransformComponent>().Rotation;
	}

	static void TransformComponent_SetRotation(uint32_t eid, glm::vec3* inRotation) {
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityFromID(eid);
		entity.GetComponent<TransformComponent>().Rotation = *inRotation;
	}

	static void TransformComponent_GetScale(uint32_t eid, glm::vec3* outScale) {
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityFromID(eid);
		*outScale = entity.GetComponent<TransformComponent>().Scale;
	}

	static void TransformComponent_SetScale(uint32_t eid, glm::vec3* inScale) {
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityFromID(eid);
		entity.GetComponent<TransformComponent>().Scale = *inScale;
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

	static void RigidBody2DComponent_SetTranslation(uint32_t eid, glm::vec2* translation) {
		Scene* scene = ScriptEngine::GetSceneContext();
		FL_CORE_ASSERT(scene != nullptr);
		Entity entity = scene->GetEntityFromID(eid);
		FL_CORE_ASSERT(entity != nullptr);

		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		PhysicsUtils::WarpBody(rb2d.RuntimeBody, *translation);
	}

	static uint32_t Entity_FindEntityByName(MonoString* name) {
		Scene* scene = ScriptEngine::GetSceneContext();
		auto& entities = scene->GetEntitiesByTag(mono_string_to_utf8(name));
		if (entities.size() == 0) return 0;
		return entities[0];
	}

	static MonoObject* Entity_GetScriptInstance(uint32_t id) {
		return ScriptEngine::GetManagedInstance(id);
	}

	static MonoString* TagComponent_GetTag(uint32_t eid) {
		Scene* scene = ScriptEngine::GetSceneContext();
		FL_CORE_ASSERT(scene != nullptr);
		Entity entity = scene->GetEntityFromID(eid);
		FL_CORE_ASSERT(entity != nullptr);
		return mono_string_new(mono_get_root_domain(), entity.GetComponent<TagComponent>().Tag.c_str());
	}

	static void TagComponent_SetTag(uint32_t eid, MonoString* tag) {
		Scene* scene = ScriptEngine::GetSceneContext();
		FL_CORE_ASSERT(scene != nullptr);
		Entity entity = scene->GetEntityFromID(eid);
		FL_CORE_ASSERT(entity != nullptr);
		char* string = mono_string_to_utf8(tag);
		entity.GetComponent<TagComponent>().Tag = string;
		mono_free(string);
	}

	void ScriptGlue::RegisterFunctions() {
		FL_ADD_INTERNAL_CALL(CoreTrace);
		FL_ADD_INTERNAL_CALL(Input_IsKeyDown);
		FL_ADD_INTERNAL_CALL(Entity_HasComponent);
		FL_ADD_INTERNAL_CALL(Entity_FindEntityByName);
		FL_ADD_INTERNAL_CALL(Entity_GetScriptInstance);

		//Component functions
		FL_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		FL_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		FL_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		FL_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		FL_ADD_INTERNAL_CALL(TransformComponent_GetScale);
		FL_ADD_INTERNAL_CALL(TransformComponent_SetScale);
		FL_ADD_INTERNAL_CALL(RigidBody2DComponent_SetTranslation);
		FL_ADD_INTERNAL_CALL(TagComponent_GetTag);
		FL_ADD_INTERNAL_CALL(TagComponent_SetTag);
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
		s_HasComponentFunctions.clear();
		RegisterComponent(AllComponents{});
	}
}