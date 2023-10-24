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
    /// Note: components exist in the flora backend beyond the script engine! Think
    /// of a component as a "Door" to the flora backend component, not the actual component
    /// itself. Treat this component more like a *reference* to the backend component.
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
    /// Perspective cameras are similar to how we view the world in 3D space, with perspective.
    /// Orthographic cameras capture 2D images with no sense of perspective.
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
        /// The entity that this rigid body is colliding with. If there is no collision,
        /// this will be null. Note that this is a readonly value, as the developer cannot
        /// enforce a collision by setting this.
        /// </summary>
        public Entity Collision
        {
            get
            {
                long interim = InternalCalls.RigidBody2DComponent_GetCollision(Entity.ID);
                if (interim < 0) return null;
                return new Entity((uint)interim);
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

        /// <summary>
        /// Determines whether this entity's visibility is inherited from the parent's sprite component
        /// </summary>
        public bool InheritSpriteProperties
        {
            get
            {
                return InternalCalls.ParentComponent_GetInheritSpriteProperties(Entity.ID);
            }
            set
            {
                InternalCalls.ParentComponent_SetInheritSpriteProperties(Entity.ID, value);
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
        /// Gets how many children the entity has.
        /// </summary>
        /// <returns>The number of child entities</returns>
        public uint GetChildrenSize()
        {
            return InternalCalls.ChildComponent_GetChildrenSize(Entity.ID);
        }

        /// <summary>
        /// Gets the child given an index.
        /// </summary>
        /// <param name="index">The nth child of this entity</param>
        /// <returns>The child requested or null if it was not found</returns>
        public Entity GetChild(uint index)
        {
            long entityID = InternalCalls.ChildComponent_GetChild(Entity.ID, index);
            if (entityID < 0) return null;
            return Scene.GetEntityByID((uint)entityID);
        }
    }

    /// <summary>
    /// Expirimental/Dangerous. DO NOT USE.
    /// </summary>
    public class ScriptManagerComponent : Component { }

    /// <summary>
    /// Expirimental/Dangerous. DO NOT USE.
    /// </summary>
    public class ScriptComponent : Component { }

    /// <summary>
    /// A camera that can be used to view the current scene
    /// </summary>
    public class CameraComponent : Component
    {
        /// <summary>
        /// Determines if the camera uses a fixed aspect ratio. (can you stretch the viewport)
        /// </summary>
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

        /// <summary>
        /// Determines if the camera border is shown.
        /// </summary>
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

        /// <summary>
        /// Determines the orthographic size of the camera. (horizontal size)
        /// </summary>
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

        /// <summary>
        /// Determines the cutoffpoint in the -Z direction. Once an entity is behind
        /// this boundary, the entity will no longer be able to be seen by the orthographic camera.
        /// </summary>
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

        /// <summary>
        /// Determines the cutoffpoint in the Z direction. Once an entity is in front of
        /// this boundary, the entity will no longer be able to be seen by the orthographic camera.
        /// </summary>
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

        /// <summary>
        /// Determines the field of view for a perspective camera
        /// </summary>
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

        /// <summary>
        /// Determines the cutoffpoint in the -Z direction. Once an entity is behind
        /// this boundary, the entity will no longer be able to be seen by the perspective camera.
        /// </summary>
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

        /// <summary>
        /// Determines the cutoffpoint in the Z direction. Once an entity is in front of
        /// this boundary, the entity will no longer be able to be seen by the perspective camera.
        /// </summary>
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

        /// <summary>
        /// Determines the type of camera this entity represents. For more information, check 
        /// out ProjectionType.
        /// </summary>
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

    /// <summary>
    /// Renders a circle with a flat circle shader.
    /// </summary>
    public class CircleRendererComponent : Component
    {
        /// <summary>
        /// Determines the color of the circle sprite.
        /// </summary>
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

        /// <summary>
        /// Determines the radius of the circle sprite. Scales with the entity transform.
        /// </summary>
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

        /// <summary>
        /// Determines the thickness of the circle sprite.
        /// </summary>
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

        /// <summary>
        /// Determines the fade factor of the circle sprite.
        /// </summary>
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

    /// <summary>
    /// Attatches a customizeable texture to an entity. Defaults to a blank white texture
    /// when no specified texture has been specified. If a texture has been specified yet
    /// cannot be properly found, defaults to a bright purple texture.
    /// </summary>
    public class SpriteRendererComponent : Component
    {
        /// <summary>
        /// Determines if the texture is visible or not.
        /// </summary>
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

        /// <summary>
        /// Determines if the texture is paused given that it is an animation.
        /// </summary>
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

        /// <summary>
        /// Determines the type of texture being represetned. For more information, check out
        /// SpriteType.
        /// </summary>
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

        /// <summary>
        /// Determines the color of the texture. This color will be applied as a tint to the texture.
        /// </summary>
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

        /// <summary>
        /// Determines the tiling factor of the texture. In other words, this represents
        /// how much the texutre is repeated to fill the transform. Increasing this beyond one will
        /// cause the given texture to repeat itself.
        /// </summary>
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

        /// <summary>
        /// Determines how many rows are in the texture given that it is a spritesheet.
        /// Note: indexing starts at 1.
        /// </summary>
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

        /// <summary>
        /// Determines how many columns are in the texture given that it is a spritesheet.
        /// Note: indexing starts at 1.
        /// </summary>
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

        /// <summary>
        /// Determines the current row coordinate of the texture given that it is a spritesheet.
        /// Note: indexing starts at 1.
        /// </summary>
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

        /// <summary>
        /// Determines the current column coordinate of the texture given that it is a spritesheet.
        /// Note: indexing starts at 1.
        /// </summary>
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

        /// <summary>
        /// Determines the width of the rendered subtexture given that it is from a spritesheet.
        /// </summary>
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

        /// <summary>
        /// Determines the height of the rendered subtexture given that it is from a spritesheet.
        /// </summary>
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

        /// <summary>
        /// Determines how many frames are in the sprite given that it is an animation.
        /// </summary>
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

        /// <summary>
        /// Determines the starting frame of the sprite given that it is an animation.
        /// </summary>
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

        /// <summary>
        /// Determines the last frame of the sprite given that it is an animation.
        /// </summary>
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

        /// <summary>
        /// Determines the current frame of the sprite given that it is an animation.
        /// </summary>
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

        /// <summary>
        /// Determines how many frames per second the sprite plays at given that it is an animation.
        /// </summary>
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

        /// <summary>
        /// Determines which frame the sprite is at in reference to the starting frame given that it is an animation.
        /// </summary>
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

        /// <summary>
        /// Determines the path to the texture being rendered. Note: this is also used
        /// as an ID during asset management due to the innate nature of filepaths being globally unique.
        /// </summary>
        public string Path
        {
            get
            {
                return InternalCalls.SpriteRendererComponent_GetPath(Entity.ID);
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetPath(Entity.ID, value);
                TextureInitialized = false;
            }
        }

        /// <summary>
        /// Determines the name of the file the texture is being rendered from.
        /// </summary>
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

        /// <summary>
        /// Determines if the texture has been initialized yet. WARNING: this is
        /// internally used to manage textures. If you do not know what you are doing, it
        /// is recommended you do not play with this.
        /// </summary>
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

    /// <summary>
    /// A component that attatches a tag to a component. Note: tags are not globally
    /// unique; flora does not enforce that each tag must be different.
    /// </summary>
    public class TagComponent : Component
    {
        /// <summary>
        /// A string that contains the tag given to the entity.
        /// </summary>
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

    /// <summary>
    /// Contains the transform information of an entity in the scene coordinate plane.
    /// </summary>
    public class TransformComponent : Component
    {
        /// <summary>
        /// The x, y, and z coordinates of the entity.
        /// </summary>
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

        /// <summary>
        /// The rotation of the entity in reference to the x, y, and z axis.
        /// </summary>
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

        /// <summary>
        /// The scale of the entity in reference to the x, y, and z cartesian plane.
        /// </summary>
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

    /// <summary>
    /// A component to interact with the labels assigned to an entity
    /// </summary>
    public class LabelComponent : Component
    {
        /// <summary>
        /// Adds a label to an entity
        /// </summary>
        /// <param name="name">the name of the label</param>
        /// <returns>False if the specified label does not exist</returns>
        bool AddLabel(string name)
        {
            return InternalCalls.LabelComponent_AddLabel(Entity.ID, name);
        }

        /// <summary>
        /// Removes a label from an entity
        /// </summary>
        /// <param name="name">the name of the label</param>
        /// <returns>False if the specified label does not exist</returns>
        bool RemoveLabel(string name)
        {
            return InternalCalls.LabelComponent_RemoveLabel(Entity.ID, name);
        }

        /// <summary>
        /// Gets if the entity has a label
        /// </summary>
        /// <param name="name">the name of the label</param>
        /// <returns>False the entity doesn't have this label</returns>
        bool HasLabel(string name)
        {
            return InternalCalls.LabelComponent_HasLabel(Entity.ID, name);
        }
    }
}
