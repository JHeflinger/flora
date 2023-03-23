#include "flpch.h"
#include "Flora/Scene/Entity.h"

namespace Flora {
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene) {}
}