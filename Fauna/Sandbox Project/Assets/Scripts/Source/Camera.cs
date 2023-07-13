using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flora;

namespace Sandbox
{
    public class Camera : Entity
    {
        public Entity player;
        private TransformComponent m_Transform;

        void OnCreate()
        {
            player = FindEntityByName("Controllable Player");
            m_Transform = GetComponent<TransformComponent>();
        }

        void OnUpdate(float ts)
        {
            if (player != null)
            {
                m_Transform.Translation = new Vector3(player.Translation.X, player.Translation.Y, 0.0f);
                Player real_player = player.As<Player>();
                real_player.Testfunc();
            }
        }

        void OnDestroy()
        {

        }
    }
}
