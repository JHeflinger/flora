#pragma once
#include "entt.hpp"
#include "Flora/Core/Timestep.h"
#include "Flora/Renderer/EditorCamera.h"

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
		void DestroyEntity(Entity entity);
		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& Camera);
		void OnViewportResize(uint32_t width, uint32_t height);
		Entity GetPrimaryCameraEntity();
		void SetSceneFilepath(const std::string& filepath) { m_SceneFilepath = filepath; }
		void SetViewportHovered(bool hovered) { m_ViewportHovered = hovered; }
		void SetViewportFocused(bool hovered) { m_ViewportFocused = hovered; }
		std::string GetSceneFilepath() { return m_SceneFilepath; }
		bool EntityExists(uint32_t entityID);
		void SetSceneName(std::string name) { m_SceneName = name; }
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
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth, m_ViewportHeight = 0;
		bool m_ViewportHovered = false; // temporary solution
		bool m_ViewportFocused = false;
		std::string m_SceneFilepath = "NULL";
		std::string m_SceneName = "Untitled";
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class Serializer;
	};
}
