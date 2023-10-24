#include "flpch.h"
#include "Entity.h"

namespace Flora {
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene), m_Initialized(true) {}
}