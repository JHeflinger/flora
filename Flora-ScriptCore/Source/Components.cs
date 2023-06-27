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
