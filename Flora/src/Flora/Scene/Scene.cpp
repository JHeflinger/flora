#include "flpch.h"
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Components.h"
#include "Flora/Renderer/Renderer2D.h"
#include <glm/glm.hpp>

namespace Flora {
	Scene::Scene() {
		
	}

	Scene::~Scene() {

	}

	entt::entity Scene::CreateEntity() {
		return m_Registry.create();
	}

	void Scene::OnUpdate(Timestep ts) {
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group) {
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}