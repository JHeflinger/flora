using System;

namespace Flora
{
    public class Entity
    {
        protected Entity() { ID = 0; }

        public float FloatVar { get; set; }

        internal Entity(uint eid)
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

        public Entity GetEntityByID(uint eid)
        {
            return new Entity(eid);
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

            T component = new T() { Entity = this };
            return component;
        }

        public Entity FindEntityByName(string name)
        {
            uint eid = InternalCalls.Entity_FindEntityByName(name);
            if (eid == 0) return null;
            return new Entity(eid);
        }

        public T As<T>() where T : Entity, new()
        {
            object instance = InternalCalls.Entity_GetScriptInstance(ID);
            return instance as T;
        }
    }
}