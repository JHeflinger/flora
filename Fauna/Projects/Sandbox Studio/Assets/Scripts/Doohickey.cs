using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flora;

namespace Sandbox
{
    public class Doohickey : Entity
    {
        private TransformComponent m_Transform;
        private ChildComponent m_Children;
        public float speed = 15.0f;
        public float variable_float;
        public Vector2 variable_vec2;
        public Vector3 variable_Vec3;
        public Vector4 variable_Vec4;
        public int variable_int;
        public uint variable_uint;
        public bool variable_bool;
        public double variable_double;
        public short variable_short;
        public byte variable_byte;
        public string variable_string;
        public Entity variable_entity;

        void OnCreate()
        {
            m_Transform = GetComponent<TransformComponent>();
            m_Children = GetComponent<ChildComponent>();
        }

        void OnDestroy()
        {
        }

        void OnUpdate(float ts)
        {
            
        }
    }
}
