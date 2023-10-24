using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flora
{
    /// <summary>
    /// Class to interact with the Flora backend scene context.
    /// </summary>
    public class Scene
    {
        /// <summary>
        /// Gets the primary camera entity set in the scene.
        /// </summary>
        /// <returns>Entity containing the primary camera component</returns>
        public static Entity GetPrimaryCamera()
        {
            uint eid = InternalCalls.Scene_GetPrimaryCamera();
            return GetEntityByID(eid);
        }

        /// <summary>
        /// Creates a duplicate of an Entity into the scene context
        /// </summary>
        /// <param name="entity">Entity to copy</param>
        /// <returns>The copy of the Entity</returns>
        public static Entity CopyEntity(Entity entity)
        {
            uint eid = InternalCalls.Scene_CopyEntity(entity.ID);
            return GetEntityByID(eid);
        }

        /// <summary>
        /// Creates an Entity in the scene context given a name. Defaults to "Untitled Entity" if an empty string is provided.
        /// Note: this is the safest and most recommended way to create an entity, as it directly creates the Entity in the scene 
        /// context via the Flora backend.
        /// </summary>
        /// <param name="name">name of the Entity</param>
        /// <returns>created entity</returns>
        public static Entity CreateEntity(string name = "")
        {
            uint eid = InternalCalls.Scene_CreateEntity(name);
            return GetEntityByID(eid);
        }

        /// <summary>
        /// Creates an Entity of a certain Entity type in the scene context. This can be used to initialize an Entity with a
        /// script component and script class. Note: this is the safest and most recommended way to create an entity, as it directly creates the Entity in the scene 
        /// context via the Flora backend.
        /// </summary>
        /// <typeparam name="T">type of the Entity</typeparam>
        /// <param name="name">name of the Entity</param>
        /// <returns>created entity</returns>
        public static T CreateEntity<T>(string name = "") where T : Entity, new()
        {
            Type entityType = typeof(T);
            uint eid = InternalCalls.Scene_CreateScriptEntity(entityType.ToString(), name);
            return GetEntityByID(eid).As<T>();
        }

        /// <summary>
        /// Destroys an Entity in the scene context. Note: this is the only way to properly
        /// destroy an entity. Destroying an entity object typically will just remove it from the
        /// script engine; the entity will still exist in the scene context unless you use this
        /// method to touch the Flora backend and delete the entity.
        /// </summary>
        /// <param name="entity">the Entity to delete</param>
        public static void DestroyEntity(Entity entity)
        {
            InternalCalls.Scene_DestroyEntity(entity.ID);
        }

        /// <summary>
        /// Finds the first entity with the given name. Note: multiple entities can have the same name;
        /// Flora does not enforce required uniquess to names, so this function wil just return the first
        /// entity it finds.
        /// </summary>
        /// <param name="name">the name of the Entity to find</param>
        /// <returns>the found Entity, or null if it was unable to find.</returns>
        public static Entity FindEntityByName(string name)
        {
            long eid = InternalCalls.Entity_FindEntityByName(name);
            return new Entity((uint)eid);
        }

        /// <summary>
        /// Gets an entity given an ID. 
        /// </summary>
        /// <param name="eid">ID of the entity to get</param>
        /// <returns>The found entity, or null if the entity does not exist with that ID</returns>
        public static Entity GetEntityByID(uint eid)
        {
            return new Entity((uint)eid);
        }

        /// <summary>
        /// Determines whether an entity exists in the scene context. This can
        /// be useful as a security measure to ensure an entity you are working with
        /// is valid. For example, if an entity gets destroyed from the scene backend
        /// via Scene.DestroyEntity(Entity entity), any current script engine instance of
        /// that entity will technically still "exist", but it may error out if you try and use
        /// it since the entity reference no longer points to a valid flora backend entity. Using
        /// this function can ensure that you only handle entities when they exist.
        /// </summary>
        /// <param name="entity">Entity to check if it exists</param>
        /// <returns>true if the entity exists, false otherwise.</returns>
        public static bool EntityExists(Entity entity)
        {
            if (entity == null) return false;
            return InternalCalls.Scene_EntityExists(entity.ID);
        }
    }
}
