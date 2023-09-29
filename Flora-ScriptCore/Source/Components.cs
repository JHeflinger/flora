using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;

namespace Flora
{
    /// <summary>
    /// An abstract component class that can be attatched to entities.
    /// By packaging functionality into a component, entities can then 
    /// attach these components to dynamically create custom entities with ease.
    /// </summary>
    public abstract class Component
    {
        public Entity Entity { get; set; }
    }

    /// <summary>
    /// Enumeration to indicate what type the sprite is.
    /// SINGLE is a static single sprite that uses the entire given texture.
    /// SUBTEXTURE is a subsection cut from the given texture.
    /// ANIMATION is an animated sprite given a spritesheet texture.
    /// </summary>
    public enum SpriteType
    {
        SINGLE = 0,
        SUBTEXTURE = 1,
        ANIMATION = 2
    }

    /// <summary>
    /// Enumeration to indicate the projection type of the camera: Perspective or Orthographic.
    /// </summary>
    public enum ProjectionType
    {
        Perspective = 0,
        Orthographic = 1
    }

    /// <summary>
    /// Enumeration to indicate a type of physics body. 
    /// STATIC refers to a non-moving physical entity that is not affected by outside forces.
    /// KINEMATIC refers to a phsyical entity that can move but is not affected by outside forces.
    /// DYNAMIC refers to a physical entity that can move and is affected by outside forces.
    /// </summary>
    public enum BodyType
    {
        STATIC = 0,
        KINEMATIC = 1,
        DYNAMIC = 2
    }

    /// <summary>
    /// A circular shaped collision body component. When attached to an entity, the entity
    /// is now available to be collided with given a specific circular shape. Defaults to scaling
    /// with the entity transform.
    /// </summary>
    public class CircleCollider2DComponent : Component
    {
        /// <summary>
        /// The offset of the origin of the collision body from the origin of the entity.
        /// </summary>
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

        /// <summary>
        /// The radius of the collision body's size.
        /// </summary>
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

        /// <summary>
        /// The density of the collision body. Can affect the weight and force it may
        /// have when colliding with other physics bodies.
        /// </summary>
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

        /// <summary>
        /// The friction applied to the collision body. Can affect the drag and force
        /// that may be applied when colliding with other phsyics bodies.
        /// </summary>
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

        /// <summary>
        /// The "bounciness" of a physics body. In other words, the amount of energy retained
        /// after a collision. (Example: a tennis ball with 0.5 restitution would bounce halfway
        /// back up from the ground when dropped.)
        /// </summary>
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

        /// <summary>
        /// The threshold of when to apply restitution. When the collision force of the body drops
        /// below this threshold, restitution will no longer be applied.
        /// </summary>
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

    /// <summary>
    /// A box shaped collision body component. When attached to an entity, the entity
    /// is now available to be collided with given a specific rectangular shape. Defaults to scaling
    /// with the entity transform.
    /// </summary>
    public class BoxCollider2DComponent : Component
    {
        /// <summary>
        /// The offset of the origin of the collision body from the origin of the entity.
        /// </summary>
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

        /// <summary>
        /// The size of the collision body. Scales in refereince to
        /// the original transform of the entity.
        /// </summary>
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

        /// <summary>
        /// The density of the collision body. Can affect the weight and force it may
        /// have when colliding with other physics bodies.
        /// </summary>
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

        /// <summary>
        /// The friction applied to the collision body. Can affect the drag and force
        /// that may be applied when colliding with other phsyics bodies.
        /// </summary>
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

        /// <summary>
        /// The "bounciness" of a physics body. In other words, the amount of energy retained
        /// after a collision. (Example: a tennis ball with 0.5 restitution would bounce halfway
        /// back up from the ground when dropped.)
        /// </summary>
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

        /// <summary>
        /// The threshold of when to apply restitution. When the collision force of the body drops
        /// below this threshold, restitution will no longer be applied.
        /// </summary>
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

    /// <summary>
    /// The physics body of a given entity. Controls the position and forces enacting
    /// on the physical body of an entity, and simulates it using the Flora physics engine.
    /// </summary>
    public class RigidBody2DComponent : Component
    {
        /// <summary>
        /// The translation of the physical body of the entity. This syncs up with
        /// the entity's transform. The physics body translation takes priority over
        /// the entity's transform, so when applying a new translation via this property
        /// it will override the entity transform.
        /// </summary>
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

        /// <summary>
        /// The rotation of the physical body of the entity. This syncs up with
        /// the entity's transform. The physics body translation takes priority over
        /// the entity's transform, so when applying a new rotation via this property
        /// it will override the entity transform.
        /// </summary>
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

        /// <summary>
        /// The type of of the rigid body; controls whether the body can be moved and
        /// if it can be affected by outside forces. For more info, see the documentation
        /// on BodyType.
        /// </summary>
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

        /// <summary>
        /// Dictates if the rotation of an entity can be affected by the physics engine.
        /// </summary>
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

        /// <summary>
        /// The angular velocity of the entity.
        /// </summary>
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

        /// <summary>
        /// The linear velocity of the entity.
        /// </summary>
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

        /// <summary>
        /// Applies a force vector to the entity.Note: the difference between
        /// this and impulse is that this applies a FORCE vector, meaning that the velocity
        /// will change via a creeping acceleration. Impulse is an IMPULSE vector meaning that
        /// the velocity is immediately applied.
        /// </summary>
        /// <param name="vector">The force vector to be applied</param>
        /// <param name="offset">The offset from the origin of the entity to apply the force vector</param>
        public void ApplyForce(Vector2 vector, Vector2 offset = default)
        {
            if (offset.Equals(default(Vector2)))
                offset = Vector2.Zero;

            InternalCalls.RigidBody2DComponent_ApplyForce(Entity.ID, ref vector, ref offset);
        }

        /// <summary>
        /// Applies an impulse fector to the entity. Note: the difference between
        /// this and force is that force applies a FORCE vector, meaning that the velocity
        /// will change via a creeping acceleration. This is an IMPULSE vector meaning that
        /// the velocity is immediately applied.
        /// </summary>
        /// <param name="vector">The impulse vector to be applied</param>
        /// <param name="offset">The offset from the origin of the entity to apply the impulse vector</param>
        public void ApplyImpulse(Vector2 vector, Vector2 offset = default)
        {
            if (offset.Equals(default(Vector2)))
                offset = Vector2.Zero;

            InternalCalls.RigidBody2DComponent_ApplyImpulse(Entity.ID, ref vector, ref offset);
        }

        /// <summary>
        /// Applies a torque value to the physics body. Similar to ApplyForce but for rotation.
        /// </summary>
        /// <param name="rotation">The rotation scalar of the torque</param>
        public void ApplyTorque(float rotation)
        {
            InternalCalls.RigidBody2DComponent_ApplyTorque(Entity.ID, rotation);
        }

        /// <summary>
        /// Applies an angular impulse to the physics body. Similar to ApplyImpulse but for rotation.
        /// </summary>
        /// <param name="rotation">The rotation scalar of the torque</param>
        public void ApplyAngularImpulse(float rotation)
        {
            InternalCalls.RigidBody2DComponent_ApplyAngularImpulse(Entity.ID, rotation);
        }
    }

    /// <summary>
    /// The parent component of an entity. Stores which entity is the parent of this entity.
    /// Note: one can think of the subentity concept as a doubly linked n-ary tree. Each node can
    /// have multiple sub-nodes, and all sub-nodes will point toward their parent node. It is recommended
    /// to use functions such as Entity.AddChild(Entity child) to handle parent-child relationships, as
    /// unexpected outcomes may occur with the misuse of Parent and Child components.
    /// </summary>
    public class ParentComponent : Component
    {
        /// <summary>
        /// The parent entity of this component.
        /// </summary>
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

        /// <summary>
        /// Determines whether this entity inherits all the inheritable properties of the parent entity.
        /// </summary>
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

        /// <summary>
        /// Determines whether this entity's transform is in reference to its parents transform or not.
        /// </summary>
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

    /// <summary>
    /// The parent component of an entity. Stores what entities are this entity's children.
    /// Note: one can think of the subentity concept as a doubly linked n-ary tree. Each node can
    /// have multiple sub-nodes, and all sub-nodes will point toward their parent node. It is recommended
    /// to use functions such as Entity.AddChild(Entity child) to handle parent-child relationships, as
    /// unexpected outcomes may occur with the misuse of Parent and Child components.
    /// </summary>
    public class ChildComponent : Component
    {
        /// <summary>
        /// Removes a given entity from being this entity's children. Note: this does not
        /// remove this entity from the child's ParentComponent. Use with caution.
        /// </summary>
        /// <param name="entity">The child to remove</param>
        public void RemoveChild(Entity entity)
        {
            InternalCalls.ChildComponent_RemoveChild(Entity.ID, entity.ID);
        }

        /// <summary>
        /// Adds a given entity to this entity's children. Note: this does not
        /// add this entity to the child's ParentComponent. Use with caution.
        /// </summary>
        /// <param name="entity">The child to add</param>
        public void AddChild(Entity entity)
        {
            InternalCalls.ChildComponent_AddChild(Entity.ID, entity.ID);
        }

        /// <summary>
        /// Determines whether this entity has children or not.
        /// </summary>
        /// <returns>true if this entity has children, false otherwise.</returns>
        public bool HasChildren()
        {
            return InternalCalls.ChildComponent_HasChildren(Entity.ID);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
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
