using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flora
{
    public class Visual
    {
        public static Entity GetHoveredEntity()
        {
            long entityHandle = InternalCalls.VisualUtils_GetHoveredEntity();
            if (entityHandle < 0) return null;
            Entity newEntity = Scene.GetEntityByID((uint)entityHandle);
            if (newEntity == null) return null;
            if (newEntity.HasComponent<TransformComponent>()) return newEntity;
            return null;
        }
    }
}
