using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;

namespace Flora
{
    public static class InternalCalls
	{
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static uint Scene_CopyEntity(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static long VisualUtils_GetHoveredEntity();

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool SpriteRendererComponent_GetVisible(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetVisible(uint id, bool isVisible);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static uint Scene_GetPrimaryCamera();

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void DevTools_SendCommand(string command);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static string DevTools_GetQueuedCommand();

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool DevTools_HasQueuedCommand();

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void DevTools_ResolveCommand();

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static uint DevTools_CountQueuedCommands();

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool Input_IsMouseButtonPressed(int mouseCode);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float Input_GetMouseX();

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float Input_GetMouseY();

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void Input_GetMousePosition(out Vector2 position);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CircleCollider2DComponent_GetOffset(uint eid, out Vector2 offset);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CircleCollider2DComponent_SetOffset(uint eid, ref Vector2 offset);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float CircleCollider2DComponent_GetRadius(uint eid);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CircleCollider2DComponent_SetRadius(uint eid, float radius);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float CircleCollider2DComponent_GetDensity(uint eid);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CircleCollider2DComponent_SetDensity(uint eid, float density);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float CircleCollider2DComponent_GetFriction(uint eid);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CircleCollider2DComponent_SetFriction(uint eid, float friction);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float CircleCollider2DComponent_GetRestitution(uint eid);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CircleCollider2DComponent_SetRestitution(uint eid, float restitution);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float CircleCollider2DComponent_GetRestitutionThreshold(uint eid);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CircleCollider2DComponent_SetRestitutionThreshold(uint eid, float threshold);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_GetOffset(uint eid, out Vector2 offset);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_SetOffset(uint eid, ref Vector2 offset);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_GetSize(uint eid, out Vector2 size);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_SetSize(uint eid, ref Vector2 size);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float BoxCollider2DComponent_GetDensity(uint eid);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_SetDensity(uint eid, float density);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float BoxCollider2DComponent_GetFriction(uint eid);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_SetFriction(uint eid, float friction);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float BoxCollider2DComponent_GetRestitution(uint eid);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_SetRestitution(uint eid, float restitution);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float BoxCollider2DComponent_GetRestitutionThreshold(uint eid);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void BoxCollider2DComponent_SetRestitutionThreshold(uint eid, float threshold);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody2DComponent_SetTranslation(uint eid, ref Vector2 translation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody2DComponent_SetRotation(uint id, float rotation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static int RigidBody2DComponent_GetType(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody2DComponent_SetType(uint id, int type);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool RigidBody2DComponent_GetFixedRotation(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody2DComponent_SetFixedRotation(uint id, bool fixedRotation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float RigidBody2DComponent_GetAngularVelocity(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody2DComponent_SetAngularVelocity(uint id, float angularVelocity);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody2DComponent_GetLinearVelocity(uint id, out Vector2 outVelocity);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody2DComponent_SetLinearVelocity(uint id, ref Vector2 inVelocity);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody2DComponent_ApplyForce(uint id, ref Vector2 vector, ref Vector2 offset);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody2DComponent_ApplyImpulse(uint id, ref Vector2 vector, ref Vector2 offset);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody2DComponent_ApplyTorque(uint id, float rotation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void RigidBody2DComponent_ApplyAngularImpulse(uint id, float rotation);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static uint ParentComponent_GetParent(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void ParentComponent_SetParent(uint id, uint parent);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool ParentComponent_GetInheritAll(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void ParentComponent_SetInheritAll(uint id, bool inheritAll);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool ParentComponent_GetInheritTransform(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void ParentComponent_SetInheritTransform(uint id, bool inheritTransform);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void ChildComponent_RemoveChild(uint id, uint childid);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void ChildComponent_AddChild(uint id, uint childid);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool ChildComponent_HasChildren(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static uint ChildComponent_GetChildrenSize(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static uint ChildComponent_GetChild(uint id, uint index);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool CameraComponent_GetFixedAspectRatio(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_SetFixedAspectRatio(uint id, bool ratio);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool CameraComponent_GetShowBorder(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_SetShowBorder(uint id, bool showborder);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float CameraComponent_GetOrthographicSize(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_SetOrthographicSize(uint id, float size);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float CameraComponent_GetOrthographicNear(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_SetOrthographicNear(uint id, float nearclip);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float CameraComponent_GetPerspectiveNear(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_SetPerspectiveNear(uint id, float nearclip);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float CameraComponent_GetOrthographicFar(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_SetOrthographicFar(uint id, float farclip);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float CameraComponent_GetPerspectiveFar(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_SetPerspectiveFar(uint id, float farclip);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float CameraComponent_GetPerspectiveFOV(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_SetPerspectiveFOV(uint id, float fov);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static int CameraComponent_GetProjectionType(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CameraComponent_SetProjectionType(uint id, int projectionType);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CircleRendererComponent_GetColor(uint id, out Vector4 color);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CircleRendererComponent_SetColor(uint id, ref Vector4 color);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float CircleRendererComponent_GetRadius(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CircleRendererComponent_SetRadius(uint id, float radius);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float CircleRendererComponent_GetThickness(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CircleRendererComponent_SetThickness(uint id, float thickness);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float CircleRendererComponent_GetFade(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void CircleRendererComponent_SetFade(uint id, float fade);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static SpriteType SpriteRendererComponent_GetSpriteType(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetSpriteType(uint id, int type);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_GetColor(uint id, out Vector4 color);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetColor(uint id, ref Vector4 color);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float SpriteRendererComponent_GetTilingFactor(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetTilingFactor(uint id, float tilingFactor);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static int SpriteRendererComponent_GetRows(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetRows(uint id, int rows);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static int SpriteRendererComponent_GetColumns(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetColumns(uint id, int columns);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static int SpriteRendererComponent_GetRowCoordinate(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetRowCoordinate(uint id, int rowCoordinatae);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static int SpriteRendererComponent_GetColumnCoordinate(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetColumnCoordinate(uint id, int columnCoordinate);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float SpriteRendererComponent_GetSubtextureWidth(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetSubtextureWidth(uint id, float width);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static float SpriteRendererComponent_GetSubtextureHeight(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetSubtextureHeight(uint id, float height);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static int SpriteRendererComponent_GetFrames(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetFrames(uint id, int frames);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static int SpriteRendererComponent_GetStartFrame(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetStartFrame(uint id, int startFrame);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static int SpriteRendererComponent_GetEndFrame(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetEndFrame(uint id, int endFrame);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static int SpriteRendererComponent_GetCurrentFrame(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetCurrentFrame(uint id, int currentFrame);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static int SpriteRendererComponent_GetFPS(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetFPS(uint id, int fps);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static int SpriteRendererComponent_GetFrameCounter(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetFrameCounter(uint id, int frameCounter);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static string SpriteRendererComponent_GetPath(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetPath(uint id, string path);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static string SpriteRendererComponent_GetFilename(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetFilename(uint id, string filename);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static bool SpriteRendererComponent_GetTextureInitialized(uint id);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		internal extern static void SpriteRendererComponent_SetTextureInitialized(uint id, bool initialized);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string TagComponent_SetTag(uint id, string tag);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string TagComponent_GetTag(uint id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static uint Entity_FindEntityByName(string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object Entity_GetScriptInstance(uint id);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CoreTrace(string log);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetRotation(uint eid, out Vector3 rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetRotation(uint eid, ref Vector3 rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetScale(uint eid, out Vector3 scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetScale(uint eid, ref Vector3 scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetTranslation(uint eid, out Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetTranslation(uint eid, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keycode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(uint eid, Type componentType);
    }
}
