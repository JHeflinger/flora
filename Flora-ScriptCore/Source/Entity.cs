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

        void Bind()
        {
            Scene.BindEntity(this);
        }

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

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, componentType);
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