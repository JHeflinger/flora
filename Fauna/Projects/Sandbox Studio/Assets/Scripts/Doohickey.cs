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
        private float temp = 0.0f;

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
