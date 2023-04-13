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
		void DestroyEntity(Entity entity);
		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& Camera);
		void OnViewportResize(uint32_t width, uint32_t height);
		Entity GetPrimaryCameraEntity();
		void SetSceneFilepath(const std::string& filepath) { m_SceneFilepath = filepath; }
		void SetViewportHovered(bool hovered) { m_ViewportHovered = hovered; }
		std::string GetSceneFilepath() { return m_SceneFilepath; }
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth, m_ViewportHeight = 0;
		bool m_ViewportHovered = false; // temporary solution
		std::string m_SceneFilepath = "NULL";
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class Serializer;
	};
}
