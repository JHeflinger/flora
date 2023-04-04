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
		void DestroyEntity(Entity entity);
		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& Camera);
		void OnViewportResize(uint32_t width, uint32_t height);
		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth, m_ViewportHeight = 0;
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}
