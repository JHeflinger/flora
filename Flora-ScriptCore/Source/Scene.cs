using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flora
{
    public class Scene
    {
        private static Dictionary<uint, Entity> EntityMap = new Dictionary<uint, Entity>();
        
        public static void BindEntity(Entity entity)
        {
            if (EntityMap.ContainsKey(entity.ID))
                EntityMap[entity.ID] = entity;
            else
                EntityMap.Add(entity.ID, entity);
        }

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
            if (eid < 0) return null;
            if (!EntityMap.ContainsKey((uint)eid)) return null;
            return EntityMap[(uint)eid];
        }

        public static Entity GetEntityByID(uint eid)
        {
            if (!EntityMap.ContainsKey((uint)eid))
                EntityMap.Add(eid, new Entity(eid));
            return EntityMap[(uint)eid];
        }
    }
}
