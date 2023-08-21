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
            uint eid = InternalCalls.Scene_GetPrimaryCamera();
            return GetEntityByID(eid);
        }

        public static Entity CopyEntity(Entity entity)
        {
            uint eid = InternalCalls.Scene_CopyEntity(entity.ID);
            return GetEntityByID(eid);
        }

        public static Entity FindEntityByName(string name)
        {
            long eid = InternalCalls.Entity_FindEntityByName(name);
            return new Entity((uint)eid);
        }

        public static Entity GetEntityByID(uint eid)
        {
            return new Entity((uint)eid);
        }
    }
}
