using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;

namespace Flora
{
    public abstract class Component
    {
        public Entity Entity { get; set; }
    }

    public enum SpriteType
    {
        SINGLE = 0,
        SUBTEXTURE = 1,
        ANIMATION = 2
    }

    public enum ProjectionType
    {
        Perspective = 0,
        Orthographic = 1
    }

    public enum BodyType
    {
        STATIC = 0,
        KINEMATIC = 1,
        DYNAMIC = 2
    }

    public class CircleCollider2DComponent : Component
    {
        public Vector2 Offset
        {
            get
            {
                InternalCalls.CircleCollider2DComponent_GetOffset(Entity.ID, out Vector2 offset);
                return offset;
            }
            set
            {
                InternalCalls.CircleCollider2DComponent_SetOffset(Entity.ID, ref value);
            }
        }

        public float Radius
        {
            get
            {
                return InternalCalls.CircleCollider2DComponent_GetRadius(Entity.ID);
            }
            set
            {
                InternalCalls.CircleCollider2DComponent_SetRadius(Entity.ID, value);
            }
        }

        public float Density
        {
            get
            {
                return InternalCalls.CircleCollider2DComponent_GetDensity(Entity.ID);
            }
            set
            {
                InternalCalls.CircleCollider2DComponent_SetDensity(Entity.ID, value);
            }
        }

        public float Friction
        {
            get
            {
                return InternalCalls.CircleCollider2DComponent_GetFriction(Entity.ID);
            }
            set
            {
                InternalCalls.CircleCollider2DComponent_SetFriction(Entity.ID, value);
            }
        }

        public float Restitution
        {
            get
            {
                return InternalCalls.CircleCollider2DComponent_GetRestitution(Entity.ID);
            }
            set
            {
                InternalCalls.CircleCollider2DComponent_SetRestitution(Entity.ID, value);
            }
        }

        public float RestitutionThreshold
        {
            get
            {
                return InternalCalls.CircleCollider2DComponent_GetRestitutionThreshold(Entity.ID);
            }
            set
            {
                InternalCalls.CircleCollider2DComponent_SetRestitutionThreshold(Entity.ID, value);
            }
        }
    }

    public class BoxCollider2DComponent : Component
    {
        public Vector2 Offset
        {
            get
            {
                InternalCalls.BoxCollider2DComponent_GetOffset(Entity.ID, out Vector2 offset);
                return offset;
            }
            set
            {
                InternalCalls.BoxCollider2DComponent_SetOffset(Entity.ID, ref value);
            }
        }

        public Vector2 Size
        {
            get
            {
                InternalCalls.BoxCollider2DComponent_GetSize(Entity.ID, out Vector2 size);
                return size;
            }
            set
            {
                InternalCalls.BoxCollider2DComponent_SetSize(Entity.ID, ref value);
            }
        }

        public float Density
        {
            get
            {
                return InternalCalls.BoxCollider2DComponent_GetDensity(Entity.ID);
            }
            set
            {
                InternalCalls.BoxCollider2DComponent_SetDensity(Entity.ID, value);
            }
        }

        public float Friction
        {
            get
            {
                return InternalCalls.BoxCollider2DComponent_GetFriction(Entity.ID);
            }
            set
            {
                InternalCalls.BoxCollider2DComponent_SetFriction(Entity.ID, value);
            }
        }

        public float Restitution
        {
            get
            {
                return InternalCalls.BoxCollider2DComponent_GetRestitution(Entity.ID);
            }
            set
            {
                InternalCalls.BoxCollider2DComponent_SetRestitution(Entity.ID, value);
            }
        }

        public float RestitutionThreshold
        {
            get
            {
                return InternalCalls.BoxCollider2DComponent_GetRestitutionThreshold(Entity.ID);
            }
            set
            {
                InternalCalls.BoxCollider2DComponent_SetRestitutionThreshold(Entity.ID, value);
            }
        }
    }

    public class RigidBody2DComponent : Component
    {
        public Vector2 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vector3 translation);
                return new Vector2(translation);
            }
            set
            {
                InternalCalls.RigidBody2DComponent_SetTranslation(Entity.ID, ref value);
            }
        }

        public float Rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(Entity.ID, out Vector3 rotation);
                return rotation.Z;
            }
            set
            {
                InternalCalls.RigidBody2DComponent_SetRotation(Entity.ID, value);
            }
        }

        public BodyType Type
        {
            get
            {
                return (BodyType)InternalCalls.RigidBody2DComponent_GetType(Entity.ID);
            }
            set
            {
                InternalCalls.RigidBody2DComponent_SetType(Entity.ID, (int)value);
            }
        }

        public bool FixedRotation
        {
            get
            {
                return InternalCalls.RigidBody2DComponent_GetFixedRotation(Entity.ID);
            }
            set
            {
                InternalCalls.RigidBody2DComponent_SetFixedRotation(Entity.ID, value);
            }
        }

        public float AngularVelocity
        {
            get
            {
                return InternalCalls.RigidBody2DComponent_GetAngularVelocity(Entity.ID);
            }
            set
            {
                InternalCalls.RigidBody2DComponent_SetAngularVelocity(Entity.ID, value);
            }
        }

        public Vector2 LinearVelocity
        {
            get
            {
                InternalCalls.RigidBody2DComponent_GetLinearVelocity(Entity.ID, out Vector2 vector);
                return vector;
            }
            set
            {
                InternalCalls.RigidBody2DComponent_SetLinearVelocity(Entity.ID, ref value);
            }
        }

        public void ApplyForce(Vector2 vector, Vector2 offset = default)
        {
            if (offset.Equals(default(Vector2)))
                offset = Vector2.Zero;

            InternalCalls.RigidBody2DComponent_ApplyForce(Entity.ID, ref vector, ref offset);
        }

        public void ApplyImpulse(Vector2 vector, Vector2 offset = default)
        {
            if (offset.Equals(default(Vector2)))
                offset = Vector2.Zero;

            InternalCalls.RigidBody2DComponent_ApplyImpulse(Entity.ID, ref vector, ref offset);
        }

        public void ApplyTorque(float rotation)
        {
            InternalCalls.RigidBody2DComponent_ApplyTorque(Entity.ID, rotation);
        }

        public void ApplyAngularImpulse(float rotation)
        {
            InternalCalls.RigidBody2DComponent_ApplyAngularImpulse(Entity.ID, rotation);
        }
    }

    public class ParentComponent : Component
    {
        public Entity Parent
        {
            get
            {
                return Scene.GetEntityByID(InternalCalls.ParentComponent_GetParent(Entity.ID));
            }
            set
            {
                InternalCalls.ParentComponent_SetParent(Entity.ID, value.ID);
            }
        }

        public bool InheritAll
        {
            get
            {
                return InternalCalls.ParentComponent_GetInheritAll(Entity.ID);
            }
            set
            {
                InternalCalls.ParentComponent_SetInheritAll(Entity.ID, value);
            }
        }

        public bool InheritTransform
        {
            get
            {
                return InternalCalls.ParentComponent_GetInheritTransform(Entity.ID);
            }
            set
            {
                InternalCalls.ParentComponent_SetInheritTransform(Entity.ID, value);
            }
        }
    }

    public class ChildComponent : Component
    {
        public void RemoveChild(Entity entity)
        {
            InternalCalls.ChildComponent_RemoveChild(Entity.ID, entity.ID);
        }

        public void AddChild(Entity entity)
        {
            InternalCalls.ChildComponent_AddChild(Entity.ID, entity.ID);
        }

        public bool HasChildren()
        {
            return InternalCalls.ChildComponent_HasChildren(Entity.ID);
        }

        public uint GetChildrenSize()
        {
            return InternalCalls.ChildComponent_GetChildrenSize(Entity.ID);
        }

        public Entity GetChild(uint index)
        {
            long entityID = InternalCalls.ChildComponent_GetChild(Entity.ID, index);
            if (entityID < 0) return null;
            return Scene.GetEntityByID((uint)entityID);
        }
    }

    public class ScriptManagerComponent : Component { }

    public class ScriptComponent : Component { }

    public class CameraComponent : Component
    {
        public bool FixedAspectRatio
        {
            get
            {
                return InternalCalls.CameraComponent_GetFixedAspectRatio(Entity.ID);
            }
            set
            {
                InternalCalls.CameraComponent_SetFixedAspectRatio(Entity.ID, value);
            }
        }

        public bool ShowBorder
        {
            get
            {
                return InternalCalls.CameraComponent_GetShowBorder(Entity.ID);
            }
            set
            {
                InternalCalls.CameraComponent_SetShowBorder(Entity.ID, value);
            }
        }

        public float OrthographicSize
        {
            get
            {
                return InternalCalls.CameraComponent_GetOrthographicSize(Entity.ID);
            }
            set
            {
                InternalCalls.CameraComponent_SetOrthographicSize(Entity.ID, value);
            }
        }

        public float OrthographicNear
        {
            get
            {
                return InternalCalls.CameraComponent_GetOrthographicNear(Entity.ID);
            }
            set
            {
                InternalCalls.CameraComponent_SetOrthographicNear(Entity.ID, value);
            }
        }

        public float OrthographicFar
        {
            get
            {
                return InternalCalls.CameraComponent_GetOrthographicFar(Entity.ID);
            }
            set
            {
                InternalCalls.CameraComponent_SetOrthographicFar(Entity.ID, value);
            }
        }

        public float PerspectiveFOV
        {
            get
            {
                return InternalCalls.CameraComponent_GetPerspectiveFOV(Entity.ID);
            }
            set
            {
                InternalCalls.CameraComponent_SetPerspectiveFOV(Entity.ID, value);
            }
        }

        public float PerspectiveNear
        {
            get
            {
                return InternalCalls.CameraComponent_GetPerspectiveNear(Entity.ID);
            }
            set
            {
                InternalCalls.CameraComponent_SetPerspectiveNear(Entity.ID, value);
            }
        }

        public float PerspectiveFar
        {
            get
            {
                return InternalCalls.CameraComponent_GetPerspectiveFar(Entity.ID);
            }
            set
            {
                InternalCalls.CameraComponent_SetPerspectiveFar(Entity.ID, value);
            }
        }

        public ProjectionType Type
        {
            get
            {
                return (ProjectionType)InternalCalls.CameraComponent_GetProjectionType(Entity.ID);
            }
            set
            {
                InternalCalls.CameraComponent_SetProjectionType(Entity.ID, (int)value);
            }
        }
    }

    public class CircleRendererComponent : Component
    {
        public Vector4 Color
        {
            get
            {
                InternalCalls.CircleRendererComponent_GetColor(Entity.ID, out Vector4 color);
                return color;
            }
            set
            {
                InternalCalls.CircleRendererComponent_SetColor(Entity.ID, ref value);
            }
        }

        public float Radius
        {
            get
            {
                return InternalCalls.CircleRendererComponent_GetRadius(Entity.ID);
            }
            set
            {
                InternalCalls.CircleRendererComponent_SetRadius(Entity.ID, value);
            }
        }

        public float Thickness
        {
            get
            {
                return InternalCalls.CircleRendererComponent_GetThickness(Entity.ID);
            }
            set
            {
                InternalCalls.CircleRendererComponent_SetThickness(Entity.ID, value);
            }
        }

        public float Fade
        {
            get
            {
                return InternalCalls.CircleRendererComponent_GetFade(Entity.ID);
            }
            set
            {
                InternalCalls.CircleRendererComponent_SetFade(Entity.ID, value);
            }
        }
    }

    public class SpriteRendererComponent : Component
    {
        public bool Visible
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetVisible(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetVisible(Entity.ID, value);
            }
        }

        public bool Paused
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetPaused(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetPaused(Entity.ID, value);
            }
        }

        public SpriteType Type
        {
            get
            {
                return (SpriteType)(InternalCalls.SpriteRendererComponent_GetSpriteType(Entity.ID));
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetSpriteType(Entity.ID, (int)value);
            }
        }

        public Vector4 Color
        {
            get
            {
                InternalCalls.SpriteRendererComponent_GetColor(Entity.ID, out Vector4 color);
                return color;
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetColor(Entity.ID, ref value);
            }
        }

        public float TilingFactor
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetTilingFactor(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetTilingFactor(Entity.ID, value);
            }
        }

        public int Rows
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetRows(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetRows(Entity.ID, value);
            }
        }

        public int Columns
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetColumns(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetColumns(Entity.ID, value);
            }
        }

        public int RowCoordinate
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetRowCoordinate(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetRowCoordinate(Entity.ID, value);
            }
        }

        public int ColumnCoordinate
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetColumnCoordinate(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetColumnCoordinate(Entity.ID, value);
            }
        }

        public float SubtextureWidth
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetSubtextureWidth(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetSubtextureWidth(Entity.ID, value);
            }
        }

        public float SubtextureHeight
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetSubtextureHeight(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetSubtextureHeight(Entity.ID, value);
            }
        }

        public int Frames
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetFrames(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetFrames(Entity.ID, value);
            }
        }

        public int StartFrame
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetStartFrame(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetStartFrame(Entity.ID, value);
            }
        }

        public int EndFrame
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetEndFrame(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetEndFrame(Entity.ID, value);
            }
        }

        public int CurrentFrame
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetCurrentFrame(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetCurrentFrame(Entity.ID, value);
            }
        }

        public int FPS
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetFPS(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetFPS(Entity.ID, value);
            }
        }

        public int FrameCounter
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetFrameCounter(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetFrameCounter(Entity.ID, value);
            }
        }

        public string Path
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetPath(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetPath(Entity.ID, value);
            }
        }

        public string Filename
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetFilename(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetFilename(Entity.ID, value);
            }
        }

        public bool TextureInitialized
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetTextureInitialized(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetTextureInitialized(Entity.ID, value);
            }
        }
    }

    public class TagComponent : Component
    {
        public string Tag
        {
            get
            {
                return InternalCalls.TagComponent_GetTag(Entity.ID);
            }
            set
            {
                InternalCalls.TagComponent_SetTag(Entity.ID, value);
            }
        }
    }

    public class TransformComponent : Component
    {
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vector3 translation);
                return translation;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(Entity.ID, ref value);
            }
        }

        public Vector3 Rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(Entity.ID, out Vector3 rotation);
                return rotation;
            }
            set
            {
                InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value);
            }
        }

        public Vector3 Scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(Entity.ID, out Vector3 scale);
                return scale;
            }
            set
            {
                InternalCalls.TransformComponent_SetScale(Entity.ID, ref value);
            }
        }
    }

}
