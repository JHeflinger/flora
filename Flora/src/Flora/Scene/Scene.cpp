#include "flpch.h"
#include "Flora/Scene/Scene.h"
#include "Flora/Renderer/Renderer2D.h"
#include "Flora/Scene/Entity.h"
#include <glm/glm.hpp>
#include <filesystem>
#include "Flora/Scene/Components.h"

// temp until stable alternative is implemented
#include "../../Fauna/assets/scripts/MasterNativeScript.h"

namespace Flora {
	static void DrawEntitySprite(Entity& entity, bool useTransformRef = false, glm::mat4 refTransform = glm::mat4(0.0f)) {
		glm::mat4 transform = entity.GetComponent<TransformComponent>().GetTransform();
		if (useTransformRef) transform = refTransform * transform;
		Renderer2D::DrawSprite(transform, entity.GetComponent<SpriteRendererComponent>(), (int)(uint32_t)entity);
		if (entity.HasComponent<ChildComponent>()) {
			std::vector<Entity> children = entity.GetComponent<ChildComponent>().Children;
			for (int i = 0; i < children.size(); i++) {
				if (children[i].HasComponent<SpriteRendererComponent>()) {
					bool useParentTransform = children[i].GetComponent<ParentComponent>().InheritAll || (!children[i].GetComponent<ParentComponent>().InheritAll && children[i].GetComponent<ParentComponent>().InheritTransform);
					DrawEntitySprite(children[i], useParentTransform, transform);
				}
			}
		}
	}

	Scene::Scene() {
	}

	Scene::~Scene() {
	}

	std::vector<Entity> Scene::GetEntitiesByTag(std::string tag) {
		std::vector<Entity> entities;
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view) {
			Entity foundEntity = Entity{ entity, this };
			if (foundEntity.GetComponent<TagComponent>().Tag == tag)
				entities.emplace_back(foundEntity);
		}
		return entities;
	}

	Entity Scene::CreateEntity(const std::string& name) {
		Entity entity(m_Registry.create(), this);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Untitled Entity" : name;
		return entity;
	}

	Entity Scene::CreateEntity(uint32_t id, const std::string& name) {
		Entity entity(m_Registry.create((entt::entity)id), this);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Untitled Entity" : name;
		return entity;
	}

	template <typename T>
	static void CopyComponent(Entity source, Entity dest) {
		if (source.HasComponent<T>())
			dest.AddComponent<T>(source.GetComponent<T>());
	}

	static void CopyAllComponents(Entity entity, Entity newEntity) {
		CopyComponent<TagComponent>(entity, newEntity);
		CopyComponent<TransformComponent>(entity, newEntity);
		CopyComponent<SpriteRendererComponent>(entity, newEntity);
		CopyComponent<CameraComponent>(entity, newEntity);
		CopyComponent<NativeScriptComponent>(entity, newEntity);
		CopyComponent<ScriptManagerComponent>(entity, newEntity);
	}

	Entity Scene::CopyEntity(Entity entity) {
		Entity newEntity(m_Registry.create(), this);
		CopyAllComponents(entity, newEntity);
		
		// copy children recursively to avoid having the same children
		if (entity.HasComponent<ChildComponent>()) {
			ChildComponent cc = entity.GetComponent<ChildComponent>();
			ChildComponent new_cc = newEntity.AddComponent<ChildComponent>();
			for (int i = 0; i < cc.Children.size(); i++) {
				new_cc.AddChild(CopyEntity(cc.Children[i], newEntity));
			}
		}

		return newEntity;
	}

	Entity Scene::CopyEntity(Entity entity, Entity parent) {
		Entity newEntity(m_Registry.create(), this);
		CopyAllComponents(entity, newEntity);

		// copy children recursively to avoid having the same children
		if (entity.HasComponent<ChildComponent>()) {
			ChildComponent cc = entity.GetComponent<ChildComponent>();
			ChildComponent new_cc = newEntity.AddComponent<ChildComponent>();
			for (int i = 0; i < cc.Children.size(); i++) {
				new_cc.AddChild(CopyEntity(cc.Children[i], newEntity));
			}
		}

		// modify parent based on parameter
		if (!parent.HasComponent<ChildComponent>()) parent.AddComponent<ChildComponent>();
		parent.GetComponent<ChildComponent>().AddChild(newEntity);
		CopyComponent<ParentComponent>(entity, newEntity);
		if (!newEntity.HasComponent<ParentComponent>()) newEntity.AddComponent<ParentComponent>();
		newEntity.GetComponent<ParentComponent>().Parent = parent;

		return newEntity;
	}

	void Scene::DestroyEntity(Entity entity) {
		m_Registry.destroy(entity);
	}

	bool Scene::EntityExists(uint32_t entityID) {
		return m_Registry.valid((entt::entity)entityID);
	}

	void Scene::UpdateScripts(Timestep ts) {
		// update native scripts
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
			if (nsc.Path != "NULL" && !nsc.Bound) {
				BindScriptToComponent(nsc, std::filesystem::path(nsc.Path).filename().stem().string());
			} else if (nsc.Bound) {
				if (!nsc.Instance) {
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->OnCreate();
				}
				nsc.Instance->OnUpdate(ts);
			}
		});

		// script manager
		m_Registry.view<ScriptManagerComponent>().each([=](auto entity, auto& smc) {
			for (int i = 0; i < smc.NativeScripts.size(); i++) {
				if (smc.NativeScripts[i].Path != "NULL" && !smc.NativeScripts[i].Bound) {
					BindScriptToComponent(smc.NativeScripts[i], std::filesystem::path(smc.NativeScripts[i].Path).filename().stem().string());
				} else if (smc.NativeScripts[i].Bound) {
					if (!smc.NativeScripts[i].Instance) {
						smc.NativeScripts[i].Instance = smc.NativeScripts[i].InstantiateScript();
						smc.NativeScripts[i].Instance->m_Entity = Entity{ entity, this };
						smc.NativeScripts[i].Instance->OnCreate();
					}
					smc.NativeScripts[i].Instance->OnUpdate(ts);
				}
			}
		});
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera) {
		// Render 2D Sprites
		{
			Renderer2D::BeginScene(camera);
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group) {
				Entity drawEntity = Entity{ entity, this };
				if (!drawEntity.HasComponent<ParentComponent>())
					DrawEntitySprite(drawEntity);
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateRuntime(Timestep ts) {
		// Update Scripts
		UpdateScripts(ts);

		// Render 2D Sprites
		{
			Camera* primaryCamera = nullptr;
			glm::mat4 cameraTransform;
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view) {
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.Primary) {
					primaryCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
			if (primaryCamera) {
				Renderer2D::BeginScene(primaryCamera->GetProjection(), cameraTransform);
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group) {
					Entity drawEntity = Entity{ entity, this };
					if (!drawEntity.HasComponent<ParentComponent>())
						DrawEntitySprite(drawEntity);
				}
				Renderer2D::EndScene();
			}
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non fixed aspect ratio cameras!
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio) 
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	Entity* Scene::GetPrimaryCamera() {
		return m_PrimaryCamera;
	}

	Entity Scene::GetEntityFromID(uint32_t id) {
		return Entity{ (entt::entity)id, this };
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component) {

	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component) {

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component) {

	}

	template<>
	void Scene::OnComponentAdded<ChildComponent>(Entity entity, ChildComponent& component) {

	}

	template<>
	void Scene::OnComponentAdded<ParentComponent>(Entity entity, ParentComponent& component) {

	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component) {

	}

	template<>
	void Scene::OnComponentAdded<ScriptManagerComponent>(Entity entity, ScriptManagerComponent& component) {

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}
}