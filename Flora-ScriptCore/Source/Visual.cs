using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flora
{
    /// <summary>
    /// Singleton class that specifies in visual functionality.
    /// </summary>
    public class Visual
    {
        /// <summary>
        /// Gets the Entity that the mouse is hovering over, or returns
        /// null if the mouse is not hovering over an Entity.
        /// </summary>
        /// <returns>hovered Entity or -1</returns>
        public static Entity GetHoveredEntity()
        {
            long entityHandle = InternalCalls.VisualUtils_GetHoveredEntity();
            if (entityHandle < 0) return null;
            Entity newEntity = Scene.GetEntityByID((uint)entityHandle);
            if (newEntity == null) return null;
            if (newEntity.HasComponent<TransformComponent>()) return newEntity;
            return null;
        }

        /// <summary>
        /// Gets the hovered Entity's ID, or returns -1 if the mouse
        /// is not hovering over an entity.
        /// </summary>
        /// <returns>hovered Entity ID or -1</returns>
        public static long GetHoveredEntityID()
        {
            return InternalCalls.VisualUtils_GetHoveredEntity();
        }
    }
}
