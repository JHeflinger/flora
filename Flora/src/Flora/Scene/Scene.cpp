#include "flpch.h"
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Components.h"
#include "Flora/Renderer/Renderer2D.h"
#include "Flora/Scene/Entity.h"
#include <glm/glm.hpp>

namespace Flora {
	Scene::Scene() {
		
	}

	Scene::~Scene() {

	}

	Entity Scene::CreateEntity(const std::string& name) {
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Untitled Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(Timestep ts) {
		// Render 2D Sprites
		Camera* primaryCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		auto group = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : group) {
			auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);
			if (camera.Primary) {
				primaryCamera = &camera.Camera;
				cameraTransform = &transform.Transform;
				break;
			}
		}
		if (primaryCamera) {
			Renderer2D::BeginScene(primaryCamera->GetProjection(), *cameraTransform);
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group) {
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform.Transform, sprite.Color);
			}
			Renderer2D::EndScene();
		}
	}
}