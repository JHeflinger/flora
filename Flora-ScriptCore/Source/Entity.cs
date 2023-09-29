using System;
using System.Collections.Generic;

namespace Flora
{
    /// <summary>
    /// A blank Entity object that is simulated in the scene. A Component can be
    /// attatched to an Entity to customize the functionality of an Entity dynamically,
    /// allowing complete custom and dynamic control to make game objects. Note that Entity
    /// objects exist in the Flora backend, and this class just references a "door" to access
    /// that already existing Entity in the backend. Creating an Entity object is more akin
    /// to opening a door to access the Entity, not creating the Entity itself. Treat this Entity
    /// more like a *reference* to the backend Entity.
    /// </summary>
    public class Entity
    {
        /// <summary>
        /// Creates a blank uninitialized Entity. WARNING: it is not recommended that blank entities
        /// are created this way, as it does not create it in the context of the scene. Please use the
        /// CreateEntity function from the Scene class instead.
        /// </summary>
        protected Entity() { ID = 0; }

        /// <summary>
        /// Creates an Entity given it's specified ID. WARNING: that this is indented to be used to get a corresponding existing
        /// Entity with an ID. However, do not use this to create new entities as it does not create them in the context of
        /// the scene.
        /// </summary>
        /// <param name="eid"></param>
        public Entity(uint eid)
        {
            ID = eid;
        }

        /// <summary>
        /// The ID of the Entity
        /// </summary>
        public readonly uint ID;

        /// <summary>
        /// The x, y, and z translation of the Entity in the Scene's cartesian plane.
        /// </summary>
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(ID, out Vector3 result);
                return result;
            }

            set
            {
                InternalCalls.TransformComponent_SetTranslation(ID, ref value);
            }
        }

        /// <summary>
        /// The rotation of the Entity in reference to the x, y, and z axis.
        /// </summary>
        public Vector3 Rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(ID, out Vector3 result);
                return result;
            }

            set
            {
                InternalCalls.TransformComponent_SetRotation(ID, ref value);
            }
        }

        /// <summary>
        /// The scale of the Entity in reference to the x, y, and z cartesian planes.
        /// </summary>
        public Vector3 Scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(ID, out Vector3 result);
                return result;
            }

            set
            {
                InternalCalls.TransformComponent_SetScale(ID, ref value);
            }
        }

        /// <summary>
        /// Adds a child to the Entity as a subentity. Note: this is the recommended
        /// way to create subentities as it ensures the ParentComponent and ChildComponents 
        /// are safely handled and linked.
        /// </summary>
        /// <param name="entity">The child Entity</param>
        public void AddChild(Entity entity)
        {
            if (!entity.HasComponent<ParentComponent>())
                entity.AddComponent<ParentComponent>();
            entity.GetComponent<ParentComponent>().Parent = this;
            if (!HasComponent<ChildComponent>())
                AddComponent<ChildComponent>();
            InternalCalls.ChildComponent_AddChild(ID, entity.ID);
        }

        /// <summary>
        /// Determines if the Entity has a given component. Note: remember that entities exist in 
        /// the backend beyond the script engine! If you "create" a new Entity that has an existing ID,
        /// it is still referencing an existing backend Entity and therefore it may have existing components.
        /// </summary>
        /// <typeparam name="T">The type of the component</typeparam>
        /// <returns>true of the Entity has the component, false otherwise</returns>
        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, componentType);
        }

        /// <summary>
        /// Adds a new given component given the component type. Note: remember that entities
        /// exist in the backend beyond the script engine! This function will directly access the
        /// Entity in the backend, assigning it a new component.
        /// </summary>
        /// <typeparam name="T">The type of the component</typeparam>
        public void AddComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            InternalCalls.Entity_AddComponent(ID, componentType);
        }

        /// <summary>
        /// Gets a component from the Entity given its type. If it does not have that component, return null.
        /// Note: components are also in the flora backend, existing beyond the script engine! Treat this component
        /// as you would a reference to the backend component.
        /// </summary>
        /// <typeparam name="T">The type of the component</typeparam>
        /// <returns>The component object</returns>
        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
                return null;

            T component = new T() { Entity = new Entity(ID) };
            return component;
        }

        /// <summary>
        /// Gets an Entity as a specified type. This function is to be used to get the corresponding script engine instance
        /// of the Entity as a custom type. For example, if an Entity has a Script Component "Game.Player", you can get the
        /// Entity script instance as the Player type via Entity.As<Player>().
        /// </summary>
        /// <typeparam name="T">The type of the Entity instance</typeparam>
        /// <returns>The Entity instance</returns>
        public T As<T>() where T : Entity, new()
        {
            object instance = InternalCalls.Entity_GetScriptInstance(ID);
            return instance as T;
        }
    }
}