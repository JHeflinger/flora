using System;
using System.Collections.Generic;

namespace Flora
{
    public class Entity
    {
        protected Entity() { ID = 0; }

        public float FloatVar { get; set; }

        public Entity(uint eid)
        {
            ID = eid;
        }

        public readonly uint ID;

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

        public void AddChild(Entity entity)
        {
            entity.GetComponent<ParentComponent>().Parent = this;
            if (!HasComponent<ChildComponent>())
                AddComponent<ChildComponent>();
            InternalCalls.ChildComponent_AddChild(ID, entity.ID);
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, componentType);
        }

        public void AddComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            InternalCalls.Entity_AddComponent(ID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
                return null;

            T component = new T() { Entity = new Entity(ID) };
            return component;
        }

        public T As<T>() where T : Entity, new()
        {
            object instance = InternalCalls.Entity_GetScriptInstance(ID);
            return instance as T;
        }
    }
}