using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flora
{
    public class Scene
    {
        public static Entity GetPrimaryCamera()
        {
            return new Entity(InternalCalls.Scene_GetPrimaryCamera());
        }
    }
}
