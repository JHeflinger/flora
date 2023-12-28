#pragma once
#include "entt.hpp"
#include "Flora/Core/Timestep.h"
#include "Flora/Renderer/EditorCamera.h"
#include "Flora/Core/AssetManager.h"
#include "Flora/Utils/PhysicsUtils.h"

class b2World;

namespace Flora {
	class Entity;

	class Scene {
	public:
		Scene();
		~Scene();
		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntity(uint32_t id, const std::string& name = std::string());
		Entity CreateScriptEntity(const std::string& scriptName, const std::string& name = std::string());
		Entity GetEntityFromID(uint32_t id);
		Entity CopyEntity(Entity entity);
		Entity CopyEntity(Entity entity, Entity parent);
		int GetPrimaryCamera() { return m_PrimaryCameraHandle; }
		int GetPrimaryAudioListener() { return m_PrimaryAudioListenerHandle; }
		std::vector<Entity> GetEntitiesByTag(std::string tag);
		bool EntityExists(uint32_t entityID);
		bool IsScenePaused() { return m_Paused; }
		bool IsSceneRunning() { return m_Running; }
		void DestroyEntity(Entity entity);
		void OnRuntimeStart();
		void OnRuntimeStop();
		void StepScene(uint32_t steps = 1);
		void OnUpdateRuntime(Timestep ts);
		void OnUpdateRuntime(Timestep ts, glm::mat4 viewProjection);
		void OnUpdateEditor(Timestep ts, glm::mat4 viewProjection);
		void StartPhysics();
		void UpdateScripts(Timestep ts);
		void UpdatePhysics(Timestep ts);
		void UpdateAudio();
		void RenderRuntime(Timestep ts, glm::mat4 viewProjection);
		void OnViewportResize(uint32_t width, uint32_t height);
		void SetSceneFilepath(const std::string& filepath) { m_SceneFilepath = filepath; }
		void SetViewportHovered(bool hovered) { m_ViewportHovered = hovered; }
		void SetViewportFocused(bool hovered) { m_ViewportFocused = hovered; }
		void SetSceneName(std::string name) { m_SceneName = name; }
		void SetPrimaryCamera(int camera) { m_PrimaryCameraHandle = camera; }
		void SetPrimaryAudioListener(int listener) { m_PrimaryAudioListenerHandle = listener; }
		void SetScenePaused(bool paused) { m_Paused = paused; }
		std::string GetSceneFilepath() { return m_SceneFilepath; }
		std::string GetSceneName() { return m_SceneName; }
	public:
		template<typename T, typename LoopFunction>
		void ForAllComponents(LoopFunction loopFunction);
		template<typename... Components>
		auto GetAllEntitiesWith();
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
	private:
		void DrawEntitySprite(Timestep ts, Entity& entity, bool useTransformRef = false, glm::mat4 refTransform = glm::mat4(0.0f), bool useParentVis = false, bool parentVis = true);
		void SimulateEntityPhysics(Entity& entity, bool useTransformRef = false, glm::mat4 refTransform = glm::mat4(0.0f));
		void InitializeRigidBody(Entity& entity);
		void InitializeBoxCollider(Entity& entity);
		void InitializeCircleCollider(Entity& entity);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth, m_ViewportHeight = 0;
		bool m_ViewportHovered = false; // temporary solution
		bool m_ViewportFocused = false;
		bool m_Paused = false;
		bool m_Running = false;
		std::string m_SceneFilepath = "NULL";
		std::string m_SceneName = "Untitled";
		int m_PrimaryCameraHandle = -1; //TODO: change to int64_t?
		int m_PrimaryAudioListenerHandle = -1;
		b2World* m_PhysicsWorld = nullptr;
		float m_Gravity = 9.8f;
		int32_t m_PhysicsVelocityIterations = 6;
		int32_t m_PhysicsPositionIterations = 2;
		uint32_t m_StepFrames = 0;
		std::unordered_map<b2Fixture*, Entity> m_Fixturemap;
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class Serializer;
	};
}
