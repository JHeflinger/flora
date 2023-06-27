using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flora;

namespace Sandbox
{
    public class Player : Entity
    {
        private TransformComponent m_Transform;
        private RigidBody2DComponent m_RigidBody;

        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate - {ID}");
            m_Transform = GetComponent<TransformComponent>();
            m_RigidBody = GetComponent<RigidBody2DComponent>();
            bool hasTransform = HasComponent<TransformComponent>();
            bool hasBody = HasComponent<RigidBody2DComponent>();
            Console.WriteLine($"Player has transform: {hasTransform}");
            //m_Transform.Translation = new Vector3(0.0f);
        }

        void OnDestroy()
        {
            Console.WriteLine("Player.OnDestroy");
        }

        void OnUpdate(float ts)
        {
            float speed = 15.0f;

            if (Input.IsKeyDown(KeyCode.W))
            {
                Vector3 translation = Translation;
                translation.Y += speed * ts;
                Translation = translation;
                m_RigidBody.Translation = translation.XY;
            }
            if (Input.IsKeyDown(KeyCode.A))
            {
                Vector3 translation = Translation;
                translation.X -= speed * ts;
                Translation = translation;
                m_RigidBody.Translation = translation.XY;
            }
            if (Input.IsKeyDown(KeyCode.S))
            {
                Vector3 translation = Translation;
                translation.Y -= speed * ts;
                Translation = translation;
                m_RigidBody.Translation = translation.XY;
            }
            if (Input.IsKeyDown(KeyCode.D))
            {
                Vector3 translation = Translation;
                translation.X += speed * ts;
                Translation = translation;
                m_RigidBody.Translation = translation.XY;
            }
        }
    }
}
