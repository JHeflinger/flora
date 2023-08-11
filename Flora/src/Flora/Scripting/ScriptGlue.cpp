#include "flpch.h"
#include "ScriptGlue.h"
#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"
#include "mono/jit/jit.h"
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"
#include "Flora/Scripting/ScriptEngine.h"
#include "Flora/Core/Input.h"
#include "Flora/Utils/PhysicsUtils.h"
#include "Flora/Utils/DeveloperUtils.h"
#include "Flora/Utils/VisualUtils.h"

namespace Flora {
	
	#define FL_ADD_INTERNAL_CALL(funcName) mono_add_internal_call("Flora.InternalCalls::" #funcName, funcName)

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_HasComponentFunctions;

	static Entity GetValidatedEntityFromID(uint32_t eid) {
		Scene* scene = ScriptEngine::GetSceneContext();
		FL_CORE_ASSERT(scene != nullptr);
		Entity entity = scene->GetEntityFromID(eid);
		FL_CORE_ASSERT(entity != nullptr);
		return entity;
	}

	static void CoreTrace(MonoString* monostring) {
		char* string = mono_string_to_utf8(monostring);
		FL_CORE_TRACE(string);
		mono_free(string);
	}

	static void TransformComponent_GetTranslation(uint32_t eid, glm::vec3* outTranslation) {
		Entity entity = GetValidatedEntityFromID(eid);
		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(uint32_t eid, glm::vec3* inTranslation) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<TransformComponent>().Translation = *inTranslation;
	}
	
	static void TransformComponent_GetRotation(uint32_t eid, glm::vec3* outRotation) {
		Entity entity = GetValidatedEntityFromID(eid);
		*outRotation = entity.GetComponent<TransformComponent>().Rotation;
	}

	static void TransformComponent_SetRotation(uint32_t eid, glm::vec3* inRotation) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<TransformComponent>().Rotation = *inRotation;
	}

	static void TransformComponent_GetScale(uint32_t eid, glm::vec3* outScale) {
		Entity entity = GetValidatedEntityFromID(eid);
		*outScale = entity.GetComponent<TransformComponent>().Scale;
	}

	static void TransformComponent_SetScale(uint32_t eid, glm::vec3* inScale) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<TransformComponent>().Scale = *inScale;
	}

	static bool Input_IsKeyDown(KeyCode keycode) {
		return Input::IsKeyPressed(keycode);
	}

	static bool Entity_HasComponent(uint32_t eid, MonoReflectionType* componentType) {
		Entity entity = GetValidatedEntityFromID(eid);
		MonoType* monoComponentType = mono_reflection_type_get_type(componentType);
		FL_CORE_ASSERT(s_HasComponentFunctions.find(monoComponentType) != s_HasComponentFunctions.end());
		return s_HasComponentFunctions.at(monoComponentType)(entity);
	}

	static int64_t Entity_FindEntityByName(MonoString* name) {
		Scene* scene = ScriptEngine::GetSceneContext();
		auto& entities = scene->GetEntitiesByTag(mono_string_to_utf8(name));
		if (entities.size() == 0) return -1;
		return (int64_t)((uint32_t)entities[0]);
	}

	static MonoObject* Entity_GetScriptInstance(uint32_t id) {
		return ScriptEngine::GetManagedInstance(id);
	}

	static MonoString* TagComponent_GetTag(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return mono_string_new(mono_get_root_domain(), entity.GetComponent<TagComponent>().Tag.c_str());
	}

	static void TagComponent_SetTag(uint32_t eid, MonoString* tag) {
		Entity entity = GetValidatedEntityFromID(eid);
		char* string = mono_string_to_utf8(tag);
		entity.GetComponent<TagComponent>().Tag = string;
		mono_free(string);
	}

	static int SpriteRendererComponent_GetSpriteType(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return (int)entity.GetComponent<SpriteRendererComponent>().Type;
	}

	static void SpriteRendererComponent_SetSpriteType(uint32_t eid, int type) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().Type = (SpriteRendererComponent::SpriteType)type;
	}

	static void SpriteRendererComponent_GetColor(uint32_t eid, glm::vec4* outColor) {
		Entity entity = GetValidatedEntityFromID(eid);
		*outColor = entity.GetComponent<SpriteRendererComponent>().Color;
	}

	static void SpriteRendererComponent_SetColor(uint32_t eid, glm::vec4* inColor) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().Color = *inColor;
	}

	static float SpriteRendererComponent_GetTilingFactor(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<SpriteRendererComponent>().TilingFactor;
	}

	static void SpriteRendererComponent_SetTilingFactor(uint32_t eid, float tilingFactor) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().TilingFactor = tilingFactor;
	}

	static int SpriteRendererComponent_GetRows(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<SpriteRendererComponent>().Rows;
	}

	static void SpriteRendererComponent_SetRows(uint32_t eid, int rows) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().Rows = rows;
	}

	static int SpriteRendererComponent_GetColumns(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<SpriteRendererComponent>().Columns;
	}

	static void SpriteRendererComponent_SetColumns(uint32_t eid, int columns) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().Columns = columns;
	}

	static int SpriteRendererComponent_GetRowCoordinate(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<SpriteRendererComponent>().RowCoordinate;
	}

	static void SpriteRendererComponent_SetRowCoordinate(uint32_t eid, int rowCoordinate) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().RowCoordinate = rowCoordinate;
	}

	static int SpriteRendererComponent_GetColumnCoordinate(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<SpriteRendererComponent>().ColumnCoordinate;
	}

	static void SpriteRendererComponent_SetColumnCoordinate(uint32_t eid, int columnCoordinate) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().ColumnCoordinate = columnCoordinate;
	}

	static float SpriteRendererComponent_GetSubtextureWidth(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<SpriteRendererComponent>().SubtextureWidth;
	}

	static void SpriteRendererComponent_SetSubtextureWidth(uint32_t eid, float subtextureWidth) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().SubtextureWidth = subtextureWidth;
	}

	static float SpriteRendererComponent_GetSubtextureHeight(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<SpriteRendererComponent>().SubtextureHeight;
	}

	static void SpriteRendererComponent_SetSubtextureHeight(uint32_t eid, float subtextureHeight) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().SubtextureHeight = subtextureHeight;
	}

	static int SpriteRendererComponent_GetFrames(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<SpriteRendererComponent>().Frames;
	}

	static void SpriteRendererComponent_SetFrames(uint32_t eid, int frames) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().Frames = frames;
	}

	static int SpriteRendererComponent_GetStartFrame(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<SpriteRendererComponent>().StartFrame;
	}

	static void SpriteRendererComponent_SetStartFrame(uint32_t eid, int startFrame) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().StartFrame = startFrame;
	}

	static int SpriteRendererComponent_GetEndFrame(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<SpriteRendererComponent>().EndFrame;
	}

	static void SpriteRendererComponent_SetEndFrame(uint32_t eid, int endFrame) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().EndFrame = endFrame;
	}

	static int SpriteRendererComponent_GetCurrentFrame(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<SpriteRendererComponent>().CurrentFrame;
	}

	static void SpriteRendererComponent_SetCurrentFrame(uint32_t eid, int currentFrame) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().CurrentFrame = currentFrame;
	}

	static int SpriteRendererComponent_GetFPS(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<SpriteRendererComponent>().FPS;
	}

	static void SpriteRendererComponent_SetFPS(uint32_t eid, int fps) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().FPS = fps;
	}

	static int SpriteRendererComponent_GetFrameCounter(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<SpriteRendererComponent>().FrameCounter;
	}

	static void SpriteRendererComponent_SetFrameCounter(uint32_t eid, int frameCounter) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().FrameCounter = frameCounter;
	}

	static MonoString* SpriteRendererComponent_GetPath(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return mono_string_new(mono_get_root_domain(), entity.GetComponent<SpriteRendererComponent>().Path.c_str());
	}

	static void SpriteRendererComponent_SetPath(uint32_t eid, MonoString* path) {
		Entity entity = GetValidatedEntityFromID(eid);
		char* string = mono_string_to_utf8(path);
		entity.GetComponent<SpriteRendererComponent>().Path = string;
		mono_free(string);
	}

	static MonoString* SpriteRendererComponent_GetFilename(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return mono_string_new(mono_get_root_domain(), entity.GetComponent<SpriteRendererComponent>().Filename.c_str());
	}

	static void SpriteRendererComponent_SetFilename(uint32_t eid, MonoString* filename) {
		Entity entity = GetValidatedEntityFromID(eid);
		char* string = mono_string_to_utf8(filename);
		entity.GetComponent<SpriteRendererComponent>().Filename = string;
		mono_free(string);
	}

	static bool SpriteRendererComponent_GetTextureInitialized(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<SpriteRendererComponent>().TextureInitialized;
	}

	static void SpriteRendererComponent_SetTextureInitialized(uint32_t eid, bool isInitialized) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().TextureInitialized = isInitialized;
	}

	static bool SpriteRendererComponent_GetVisible(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<SpriteRendererComponent>().Visible;
	}

	static void SpriteRendererComponent_SetVisible(uint32_t eid, bool isVisible) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<SpriteRendererComponent>().Visible = isVisible;
	}

	static void CircleRendererComponent_GetColor(uint32_t eid, glm::vec4* outColor) {
		Entity entity = GetValidatedEntityFromID(eid);
		*outColor = entity.GetComponent<CircleRendererComponent>().Color;
	}

	static void CircleRendererComponent_SetColor(uint32_t eid, glm::vec4* inColor) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CircleRendererComponent>().Color = *inColor;
	}

	static float CircleRendererComponent_GetRadius(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CircleRendererComponent>().Radius;
	}

	static void CircleRendererComponent_SetRadius(uint32_t eid, float radius) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CircleRendererComponent>().Radius = radius;
	}

	static float CircleRendererComponent_GetThickness(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CircleRendererComponent>().Thickness;
	}

	static void CircleRendererComponent_SetThickness(uint32_t eid, float thickness) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CircleRendererComponent>().Thickness = thickness;
	}

	static float CircleRendererComponent_GetFade(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CircleRendererComponent>().Fade;
	}

	static void CircleRendererComponent_SetFade(uint32_t eid, float fade) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CircleRendererComponent>().Fade = fade;
	}

	static bool CameraComponent_GetFixedAspectRatio(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CameraComponent>().FixedAspectRatio;
	}

	static void CameraComponent_SetFixedAspectRatio(uint32_t eid, bool fixedAspectRatio) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CameraComponent>().FixedAspectRatio = fixedAspectRatio;
	}

	static bool CameraComponent_GetShowBorder(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CameraComponent>().ShowBorder;
	}

	static void CameraComponent_SetShowBorder(uint32_t eid, bool showBorder) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CameraComponent>().ShowBorder = showBorder;
	}

	static float CameraComponent_GetOrthographicSize(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CameraComponent>().Camera.GetOrthographicSize();
	}

	static void CameraComponent_SetOrthographicSize(uint32_t eid, float orthographicSize) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CameraComponent>().Camera.SetOrthographicSize(orthographicSize);
	}

	static float CameraComponent_GetOrthographicNear(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CameraComponent>().Camera.GetOrthographicNearClip();
	}

	static void CameraComponent_SetOrthographicNear(uint32_t eid, float orthographicNear) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CameraComponent>().Camera.SetOrthographicNearClip(orthographicNear);
	}

	static float CameraComponent_GetOrthographicFar(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CameraComponent>().Camera.GetOrthographicFarClip();
	}

	static void CameraComponent_SetOrthographicFar(uint32_t eid, float orthographicFar) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CameraComponent>().Camera.SetOrthographicFarClip(orthographicFar);
	}

	static float CameraComponent_GetPerspectiveFOV(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CameraComponent>().Camera.GetPerspectiveVerticalFOV();
	}

	static void CameraComponent_SetPerspectiveFOV(uint32_t eid, float perspectiveFOV) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CameraComponent>().Camera.SetPerspectiveVerticalFOV(perspectiveFOV);
	}

	static float CameraComponent_GetPerspectiveNear(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CameraComponent>().Camera.GetPerspectiveNearClip();
	}

	static void CameraComponent_SetPerspectiveNear(uint32_t eid, float perspectiveNear) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CameraComponent>().Camera.SetPerspectiveNearClip(perspectiveNear);
	}

	static float CameraComponent_GetPerspectiveFar(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CameraComponent>().Camera.GetPerspectiveFarClip();
	}

	static void CameraComponent_SetPerspectiveFar(uint32_t eid, float perspectiveFar) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CameraComponent>().Camera.SetPerspectiveFarClip(perspectiveFar);
	}

	static int CameraComponent_GetProjectionType(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return (int)(entity.GetComponent<CameraComponent>().Camera.GetProjectionType());
	}

	static void CameraComponent_SetProjectionType(uint32_t eid, int projectionType) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CameraComponent>().Camera.SetProjectionType((SceneCamera::ProjectionType)projectionType);
	}

	static void ChildComponent_RemoveChild(uint32_t eid, uint32_t child) {
		Entity entity = GetValidatedEntityFromID(eid);
		Entity childEntity = GetValidatedEntityFromID(child);
		entity.GetComponent<ChildComponent>().RemoveChild(childEntity);
	}

	static void ChildComponent_AddChild(uint32_t eid, uint32_t child) {
		Entity entity = GetValidatedEntityFromID(eid);
		Entity childEntity = GetValidatedEntityFromID(child);
		entity.GetComponent<ChildComponent>().AddChild(childEntity);
	}

	static bool ChildComponent_HasChildren(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<ChildComponent>().HasChildren();
	}

	static uint32_t ChildComponent_GetChildrenSize(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<ChildComponent>().Children.size();
	}

	static int64_t ChildComponent_GetChild(uint32_t eid, uint32_t index) {
		Entity entity = GetValidatedEntityFromID(eid);
		if (entity.GetComponent<ChildComponent>().Children.size() <= index) return -1;
		return (int64_t)(uint32_t)entity.GetComponent<ChildComponent>().Children[index];
	}

	static uint32_t ParentComponent_GetParent(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return (uint32_t)entity.GetComponent<ParentComponent>().Parent;
	}

	static void ParentComponent_SetParent(uint32_t eid, uint32_t parent) {
		Entity entity = GetValidatedEntityFromID(eid);
		Entity parentEntity = GetValidatedEntityFromID(parent);
		entity.GetComponent<ParentComponent>().Parent = parentEntity;
	}

	static bool ParentComponent_GetInheritAll(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<ParentComponent>().InheritAll;
	}

	static void ParentComponent_SetInheritAll(uint32_t eid, bool inheritAll) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<ParentComponent>().InheritAll = inheritAll;
	}

	static bool ParentComponent_GetInheritTransform(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<ParentComponent>().InheritTransform;
	}

	static void ParentComponent_SetInheritTransform(uint32_t eid, bool inheritTransform) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<ParentComponent>().InheritTransform = inheritTransform;
	}

	static void RigidBody2DComponent_SetTranslation(uint32_t eid, glm::vec2* translation) {
		Entity entity = GetValidatedEntityFromID(eid);
		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		PhysicsUtils::WarpBody(rb2d.RuntimeBody, *translation);
	}

	static void RigidBody2DComponent_SetRotation(uint32_t eid, float rotation) {
		Entity entity = GetValidatedEntityFromID(eid);
		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		auto& translation = entity.GetComponent<TransformComponent>().Translation;
		PhysicsUtils::WarpBody(rb2d.RuntimeBody, { translation.x, translation.y }, rotation);
	}

	static int RigidBody2DComponent_GetType(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return (int)entity.GetComponent<RigidBody2DComponent>().Type;
	}

	static void RigidBody2DComponent_SetType(uint32_t eid, int type) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<RigidBody2DComponent>().Type = (RigidBody2DComponent::BodyType)type;
	}

	static bool RigidBody2DComponent_GetFixedRotation(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<RigidBody2DComponent>().FixedRotation;
	}

	static void RigidBody2DComponent_SetFixedRotation(uint32_t eid, bool fixedRotation) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<RigidBody2DComponent>().FixedRotation = fixedRotation;
	}

	static float RigidBody2DComponent_GetAngularVelocity(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		return PhysicsUtils::GetAngularVelocity(rb2d.RuntimeBody);
	}

	static void RigidBody2DComponent_SetAngularVelocity(uint32_t eid, float angularVelocity) {
		Entity entity = GetValidatedEntityFromID(eid);
		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		return PhysicsUtils::SetAngularVelocity(rb2d.RuntimeBody, angularVelocity);
	}

	static void RigidBody2DComponent_GetLinearVelocity(uint32_t eid, glm::vec2* outVelocity) {
		Entity entity = GetValidatedEntityFromID(eid);
		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		*outVelocity = PhysicsUtils::GetLinearVelocity(rb2d.RuntimeBody);
	}

	static void RigidBody2DComponent_SetLinearVelocity(uint32_t eid, glm::vec2* linearVelocity) {
		Entity entity = GetValidatedEntityFromID(eid);
		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		PhysicsUtils::SetLinearVelocity(rb2d.RuntimeBody, *linearVelocity);
	}

	static void RigidBody2DComponent_ApplyForce(uint32_t eid, glm::vec2* vector, glm::vec2* offset) {
		Entity entity = GetValidatedEntityFromID(eid);
		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		PhysicsUtils::ApplyForce(rb2d.RuntimeBody, *vector, *offset);
	}

	static void RigidBody2DComponent_ApplyImpulse(uint32_t eid, glm::vec2* vector, glm::vec2* offset) {
		Entity entity = GetValidatedEntityFromID(eid);
		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		PhysicsUtils::ApplyImpulse(rb2d.RuntimeBody, *vector, *offset);
	}

	static void RigidBody2DComponent_ApplyTorque(uint32_t eid, float rotation) {
		Entity entity = GetValidatedEntityFromID(eid);
		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		PhysicsUtils::ApplyTorque(rb2d.RuntimeBody, rotation);
	}

	static void RigidBody2DComponent_ApplyAngularImpulse(uint32_t eid, float rotation) {
		Entity entity = GetValidatedEntityFromID(eid);
		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		PhysicsUtils::ApplyAngularImpulse(rb2d.RuntimeBody, rotation);
	}

	static void BoxCollider2DComponent_GetOffset(uint32_t eid, glm::vec2* outOffset) {
		Entity entity = GetValidatedEntityFromID(eid);
		*outOffset = entity.GetComponent<BoxCollider2DComponent>().Offset;
	}

	static void BoxCollider2DComponent_SetOffset(uint32_t eid, glm::vec2* inOffset) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<BoxCollider2DComponent>().Offset = *inOffset;
	}

	static void BoxCollider2DComponent_GetSize(uint32_t eid, glm::vec2* outSize) {
		Entity entity = GetValidatedEntityFromID(eid);
		*outSize = entity.GetComponent<BoxCollider2DComponent>().Size;
	}

	static void BoxCollider2DComponent_SetSize(uint32_t eid, glm::vec2* inSize) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<BoxCollider2DComponent>().Size = *inSize;
	}

	static float BoxCollider2DComponent_GetDensity(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<BoxCollider2DComponent>().Density;
	}

	static void BoxCollider2DComponent_SetDensity(uint32_t eid, float density) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<BoxCollider2DComponent>().Density = density;
	}

	static float BoxCollider2DComponent_GetFriction(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<BoxCollider2DComponent>().Friction;
	}

	static void BoxCollider2DComponent_SetFriction(uint32_t eid, float friction) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<BoxCollider2DComponent>().Friction = friction;
	}

	static float BoxCollider2DComponent_GetRestitution(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<BoxCollider2DComponent>().Restitution;
	}

	static void BoxCollider2DComponent_SetRestitution(uint32_t eid, float restitution) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<BoxCollider2DComponent>().Restitution = restitution;
	}

	static float BoxCollider2DComponent_GetRestitutionThreshold(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<BoxCollider2DComponent>().RestitutionThreshold;
	}

	static void BoxCollider2DComponent_SetRestitutionThreshold(uint32_t eid, float restitutionThreshold) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<BoxCollider2DComponent>().RestitutionThreshold = restitutionThreshold;
	}

	static void CircleCollider2DComponent_GetOffset(uint32_t eid, glm::vec2* outOffset) {
		Entity entity = GetValidatedEntityFromID(eid);
		*outOffset = entity.GetComponent<CircleCollider2DComponent>().Offset;
	}

	static void CircleCollider2DComponent_SetOffset(uint32_t eid, glm::vec2* inOffset) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CircleCollider2DComponent>().Offset = *inOffset;
	}

	static float CircleCollider2DComponent_GetRadius(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CircleCollider2DComponent>().Radius;
	}

	static void CircleCollider2DComponent_SetRadius(uint32_t eid, float radius) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CircleCollider2DComponent>().Radius = radius;
	}

	static float CircleCollider2DComponent_GetDensity(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CircleCollider2DComponent>().Density;
	}

	static void CircleCollider2DComponent_SetDensity(uint32_t eid, float density) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CircleCollider2DComponent>().Density = density;
	}

	static float CircleCollider2DComponent_GetFriction(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CircleCollider2DComponent>().Friction;
	}

	static void CircleCollider2DComponent_SetFriction(uint32_t eid, float friction) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CircleCollider2DComponent>().Friction = friction;
	}

	static float CircleCollider2DComponent_GetRestitution(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CircleCollider2DComponent>().Restitution;
	}

	static void CircleCollider2DComponent_SetRestitution(uint32_t eid, float restitution) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CircleCollider2DComponent>().Restitution = restitution;
	}

	static float CircleCollider2DComponent_GetRestitutionThreshold(uint32_t eid) {
		Entity entity = GetValidatedEntityFromID(eid);
		return entity.GetComponent<CircleCollider2DComponent>().RestitutionThreshold;
	}

	static void CircleCollider2DComponent_SetRestitutionThreshold(uint32_t eid, float restitutionThreshold) {
		Entity entity = GetValidatedEntityFromID(eid);
		entity.GetComponent<CircleCollider2DComponent>().RestitutionThreshold = restitutionThreshold;
	}

	static bool Input_IsMouseButtonPressed(int mouseCode) {
		return Input::IsMouseButtonPressed((MouseCode)mouseCode);
	}

	static float Input_GetMouseX() {
		return Input::GetMouseX();
	}

	static float Input_GetMouseY() {
		return Input::GetMouseY();
	}

	static void Input_GetMousePosition(glm::vec2* position) {
		*position = Input::GetMousePosition();
	}

	static void DevTools_SendCommand(MonoString* command) {
		char* string = mono_string_to_utf8(command);
		DeveloperUtils::SendCommand(string);
		mono_free(string);
	}

	static MonoString* DevTools_GetQueuedCommand() {
		return mono_string_new(mono_get_root_domain(), DeveloperUtils::GetQueuedCommand().c_str());
	}

	static bool DevTools_HasQueuedCommand() {
		return DeveloperUtils::HasQueuedCommand();
	}

	static void DevTools_ResolveCommand() {
		DeveloperUtils::ResolveCommand();
	}

	static uint32_t DevTools_CountQueuedCommands() {
		return DeveloperUtils::CountQueuedCommands();
	}

	static uint32_t Scene_GetPrimaryCamera() {
		Scene* scene = ScriptEngine::GetSceneContext();
		return (uint32_t)(scene->GetPrimaryCamera());
	}

	static int64_t VisualUtils_GetHoveredEntity() {
		return VisualUtils::GetHoveredEntity();
	}

	static uint32_t Scene_CopyEntity(uint32_t eid) {
		Scene* scene = ScriptEngine::GetSceneContext();
		return (uint32_t)scene->CopyEntity(GetValidatedEntityFromID(eid));
	}

	void ScriptGlue::RegisterFunctions() {
		FL_ADD_INTERNAL_CALL(CoreTrace);
		FL_ADD_INTERNAL_CALL(Input_IsKeyDown);
		FL_ADD_INTERNAL_CALL(Entity_HasComponent);
		FL_ADD_INTERNAL_CALL(Entity_FindEntityByName);
		FL_ADD_INTERNAL_CALL(Entity_GetScriptInstance);
		FL_ADD_INTERNAL_CALL(Input_IsMouseButtonPressed);
		FL_ADD_INTERNAL_CALL(Input_GetMouseX);
		FL_ADD_INTERNAL_CALL(Input_GetMouseY);
		FL_ADD_INTERNAL_CALL(Input_GetMousePosition);
		FL_ADD_INTERNAL_CALL(DevTools_SendCommand);
		FL_ADD_INTERNAL_CALL(DevTools_GetQueuedCommand);
		FL_ADD_INTERNAL_CALL(DevTools_HasQueuedCommand);
		FL_ADD_INTERNAL_CALL(DevTools_ResolveCommand);
		FL_ADD_INTERNAL_CALL(DevTools_CountQueuedCommands);
		FL_ADD_INTERNAL_CALL(Scene_GetPrimaryCamera);
		FL_ADD_INTERNAL_CALL(Scene_CopyEntity);
		FL_ADD_INTERNAL_CALL(VisualUtils_GetHoveredEntity);

		//Component functions
		FL_ADD_INTERNAL_CALL(TagComponent_GetTag);
		FL_ADD_INTERNAL_CALL(TagComponent_SetTag);
		FL_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		FL_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		FL_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		FL_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		FL_ADD_INTERNAL_CALL(TransformComponent_GetScale);
		FL_ADD_INTERNAL_CALL(TransformComponent_SetScale);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetSpriteType);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetSpriteType);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetColor);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetColor);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetTilingFactor);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetTilingFactor);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetRows);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetRows);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetColumns);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetColumns);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetRowCoordinate);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetRowCoordinate);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetColumnCoordinate);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetColumnCoordinate);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetSubtextureWidth);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetSubtextureWidth);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetSubtextureHeight);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetSubtextureHeight);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetFrames);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetFrames);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetStartFrame);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetStartFrame);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetEndFrame);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetEndFrame);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetCurrentFrame);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetCurrentFrame);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetFPS);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetFPS);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetFrameCounter);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetFrameCounter);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetPath);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetPath);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetFilename);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetFilename);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetTextureInitialized);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetTextureInitialized);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_GetVisible);
		FL_ADD_INTERNAL_CALL(SpriteRendererComponent_SetVisible);
		FL_ADD_INTERNAL_CALL(CircleRendererComponent_GetColor);
		FL_ADD_INTERNAL_CALL(CircleRendererComponent_SetColor);
		FL_ADD_INTERNAL_CALL(CircleRendererComponent_SetColor);
		FL_ADD_INTERNAL_CALL(CircleRendererComponent_GetRadius);
		FL_ADD_INTERNAL_CALL(CircleRendererComponent_SetRadius);
		FL_ADD_INTERNAL_CALL(CircleRendererComponent_GetThickness);
		FL_ADD_INTERNAL_CALL(CircleRendererComponent_SetThickness);
		FL_ADD_INTERNAL_CALL(CircleRendererComponent_GetFade);
		FL_ADD_INTERNAL_CALL(CircleRendererComponent_SetFade);
		FL_ADD_INTERNAL_CALL(CameraComponent_GetFixedAspectRatio);
		FL_ADD_INTERNAL_CALL(CameraComponent_SetFixedAspectRatio);
		FL_ADD_INTERNAL_CALL(CameraComponent_GetShowBorder);
		FL_ADD_INTERNAL_CALL(CameraComponent_SetShowBorder);
		FL_ADD_INTERNAL_CALL(CameraComponent_GetOrthographicSize);
		FL_ADD_INTERNAL_CALL(CameraComponent_SetOrthographicSize);
		FL_ADD_INTERNAL_CALL(CameraComponent_GetOrthographicNear);
		FL_ADD_INTERNAL_CALL(CameraComponent_SetOrthographicNear);
		FL_ADD_INTERNAL_CALL(CameraComponent_GetPerspectiveNear);
		FL_ADD_INTERNAL_CALL(CameraComponent_SetPerspectiveNear);
		FL_ADD_INTERNAL_CALL(CameraComponent_GetOrthographicFar);
		FL_ADD_INTERNAL_CALL(CameraComponent_SetOrthographicFar);
		FL_ADD_INTERNAL_CALL(CameraComponent_GetPerspectiveFar);
		FL_ADD_INTERNAL_CALL(CameraComponent_SetPerspectiveFar);
		FL_ADD_INTERNAL_CALL(CameraComponent_GetPerspectiveFOV);
		FL_ADD_INTERNAL_CALL(CameraComponent_SetPerspectiveFOV);
		FL_ADD_INTERNAL_CALL(CameraComponent_GetProjectionType);
		FL_ADD_INTERNAL_CALL(CameraComponent_SetProjectionType);
		FL_ADD_INTERNAL_CALL(ChildComponent_RemoveChild);
		FL_ADD_INTERNAL_CALL(ChildComponent_AddChild);
		FL_ADD_INTERNAL_CALL(ChildComponent_HasChildren);
		FL_ADD_INTERNAL_CALL(ChildComponent_GetChildrenSize);
		FL_ADD_INTERNAL_CALL(ChildComponent_GetChild);
		FL_ADD_INTERNAL_CALL(ParentComponent_GetParent);
		FL_ADD_INTERNAL_CALL(ParentComponent_SetParent);
		FL_ADD_INTERNAL_CALL(ParentComponent_GetInheritAll);
		FL_ADD_INTERNAL_CALL(ParentComponent_SetInheritAll);
		FL_ADD_INTERNAL_CALL(ParentComponent_GetInheritTransform);
		FL_ADD_INTERNAL_CALL(ParentComponent_SetInheritTransform);
		FL_ADD_INTERNAL_CALL(RigidBody2DComponent_SetTranslation);
		FL_ADD_INTERNAL_CALL(RigidBody2DComponent_SetRotation);
		FL_ADD_INTERNAL_CALL(RigidBody2DComponent_GetType);
		FL_ADD_INTERNAL_CALL(RigidBody2DComponent_SetType);
		FL_ADD_INTERNAL_CALL(RigidBody2DComponent_GetFixedRotation);
		FL_ADD_INTERNAL_CALL(RigidBody2DComponent_SetFixedRotation);
		FL_ADD_INTERNAL_CALL(RigidBody2DComponent_GetAngularVelocity);
		FL_ADD_INTERNAL_CALL(RigidBody2DComponent_SetAngularVelocity);
		FL_ADD_INTERNAL_CALL(RigidBody2DComponent_GetLinearVelocity);
		FL_ADD_INTERNAL_CALL(RigidBody2DComponent_SetLinearVelocity);
		FL_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyForce);
		FL_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyImpulse);
		FL_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyTorque);
		FL_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyAngularImpulse);
		FL_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetOffset);
		FL_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetOffset);
		FL_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetSize);
		FL_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetSize);
		FL_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetDensity);
		FL_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetDensity);
		FL_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetFriction);
		FL_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetFriction);
		FL_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetRestitution);
		FL_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetRestitution);
		FL_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetRestitutionThreshold);
		FL_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetRestitutionThreshold);
		FL_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetOffset);
		FL_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetOffset);
		FL_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetRadius);
		FL_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetRadius);
		FL_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetDensity);
		FL_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetDensity);
		FL_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetFriction);
		FL_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetFriction);
		FL_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetRestitution);
		FL_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetRestitution);
		FL_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetRestitutionThreshold);
		FL_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetRestitutionThreshold);
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