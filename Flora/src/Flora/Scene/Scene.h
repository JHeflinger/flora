#pragma once
#include "entt.hpp"
#include "Flora/Core/Timestep.h"
#include "Flora/Renderer/EditorCamera.h"

class b2World;

namespace Flora {
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();
		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntity(uint32_t id, const std::string& name = std::string());
		Entity GetEntityFromID(uint32_t id);
		Entity CopyEntity(Entity entity);
		Entity CopyEntity(Entity entity, Entity parent);
		int GetPrimaryCamera() { return m_PrimaryCameraHandle; }
		std::vector<Entity> GetEntitiesByTag(std::string tag);
		bool EntityExists(uint32_t entityID);
		void DestroyEntity(Entity entity);
		void OnRuntimeStart();
		void OnRuntimeStop();
		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& Camera);
		void OnViewportResize(uint32_t width, uint32_t height);
		void SetSceneFilepath(const std::string& filepath) { m_SceneFilepath = filepath; }
		void SetViewportHovered(bool hovered) { m_ViewportHovered = hovered; }
		void SetViewportFocused(bool hovered) { m_ViewportFocused = hovered; }
		void SetSceneName(std::string name) { m_SceneName = name; }
		void SetPrimaryCamera(int camera) { m_PrimaryCameraHandle = camera; }
		std::string GetSceneFilepath() { return m_SceneFilepath; }
		std::string GetSceneName() { return m_SceneName; }
	public:
		template<typename T, typename LoopFunction>
		void ForAllComponents(LoopFunction loopFunction) {
			m_Registry.view<T>().each([=](auto entity, auto& component) {
				loopFunction(entity, component);
			});
		}
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		void UpdateScripts(Timestep ts);
		void UpdatePhysics(Timestep ts);
		void RenderRuntime();
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth, m_ViewportHeight = 0;
		bool m_ViewportHovered = false; // temporary solution
		bool m_ViewportFocused = false;
		std::string m_SceneFilepath = "NULL";
		std::string m_SceneName = "Untitled";
		int m_PrimaryCameraHandle = -1;
		b2World* m_PhysicsWorld = nullptr;
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class Serializer;
	};
}
