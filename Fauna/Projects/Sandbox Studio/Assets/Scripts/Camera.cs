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
        private TagComponent m_Tag;

        void OnCreate()
        {
            player = FindEntityByName("ball");
            m_Transform = GetComponent<TransformComponent>();
            m_Tag = GetComponent<TagComponent>();
            m_Tag.Tag = "Big Fat Ball Tracker";
        }

        void OnUpdate(float ts)
        {
            if (player != null)
            {
                m_Transform.Translation = new Vector3(player.Translation.X, player.Translation.Y, 0.0f);
                //Player real_player = player.As<Player>();
                //real_player.Testfunc();
            }
        }

        void OnDestroy()
        {

        }
    }
}
