#pragma once
#include "entt.hpp"
#include "Flora/Core/Timestep.h"
#include "Flora/Renderer/EditorCamera.h"
#include "Flora/Renderer/AssetManager.h"

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
		bool IsScenePaused() { return m_Paused; }
		void DestroyEntity(Entity entity);
		void OnRuntimeStart();
		void OnRuntimeStop();
		void StepScene(uint32_t steps = 1);
		void OnUpdateRuntime(Timestep ts);
		void OnUpdateRuntime(Timestep ts, glm::mat4 viewProjection);
		void OnUpdateEditor(Timestep ts, glm::mat4 viewProjection);
		void OnViewportResize(uint32_t width, uint32_t height);
		void SetSceneFilepath(const std::string& filepath) { m_SceneFilepath = filepath; }
		void SetViewportHovered(bool hovered) { m_ViewportHovered = hovered; }
		void SetViewportFocused(bool hovered) { m_ViewportFocused = hovered; }
		void SetSceneName(std::string name) { m_SceneName = name; }
		void SetPrimaryCamera(int camera) { m_PrimaryCameraHandle = camera; }
		void SetScenePaused(bool paused) { m_Paused = paused; }
		std::string GetSceneFilepath() { return m_SceneFilepath; }
		std::string GetSceneName() { return m_SceneName; }
		AssetManager* GetAssetManager() { return m_AssetManager; }
	public:
		template<typename T, typename LoopFunction>
		void ForAllComponents(LoopFunction loopFunction) {
			m_Registry.view<T>().each([=](auto entity, auto& component) {
				loopFunction(entity, component);
			});
		}
		template<typename... Components>
		auto GetAllEntitiesWith() {
			return m_Registry.view<Components...>();
		}
	public:
		float GetGravity() { return m_Gravity; }
		int32_t GetVelocityIterations() { return m_PhysicsVelocityIterations; }
		int32_t GetPositionIterations() { return m_PhysicsPositionIterations; }
		void SetGravity(float gravity) { m_Gravity = gravity; }
		void SetVelocityIterations(int32_t iterations) { m_PhysicsVelocityIterations = iterations; }
		void SetPositionIterations(int32_t iterations) { m_PhysicsPositionIterations = iterations; }
		float* GetGravityRef() { return &m_Gravity; }
		int32_t* GetVelocityIterationRef() { return &m_PhysicsVelocityIterations; }
		int32_t* GetPositionIterationRef() { return &m_PhysicsPositionIterations; }
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		void StartPhysics();
		void UpdateScripts(Timestep ts);
		void UpdatePhysics(Timestep ts);
		void RenderRuntime(glm::mat4 viewProjection);
	private:
		void DrawEntitySprite(Entity& entity, bool useTransformRef = false, glm::mat4 refTransform = glm::mat4(0.0f));
		void SimulateEntityPhysics(Entity& entity, bool useTransformRef = false, glm::mat4 refTransform = glm::mat4(0.0f));
		glm::mat4 GetWorldTransform(Entity& entity);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth, m_ViewportHeight = 0;
		bool m_ViewportHovered = false; // temporary solution
		bool m_ViewportFocused = false;
		bool m_Paused = false;
		std::string m_SceneFilepath = "NULL";
		std::string m_SceneName = "Untitled";
		int m_PrimaryCameraHandle = -1;
		b2World* m_PhysicsWorld = nullptr;
		float m_Gravity = 9.8f;
		int32_t m_PhysicsVelocityIterations = 6;
		int32_t m_PhysicsPositionIterations = 2;
		AssetManager* m_AssetManager;
		uint32_t m_StepFrames = 0;
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class Serializer;
	};
}
