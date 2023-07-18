using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flora
{
    public abstract class Component
    {
        public Entity Entity { get; internal set; }
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
    }
}
