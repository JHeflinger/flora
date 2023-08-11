#include "flpch.h"
#include "Scene.h"
#include "Flora/Renderer/Renderer2D.h"
#include "Flora/Scene/Entity.h"
#include "Flora/Scene/Components.h"
#include "Flora/Math/Math.h"
#include "Flora/Scripting/ScriptEngine.h"

namespace Flora {
	void Scene::DrawEntitySprite(Entity& entity, bool useTransformRef, glm::mat4 refTransform) {
		glm::mat4 transform = entity.GetComponent<TransformComponent>().GetTransform();
		if (useTransformRef) transform = refTransform * transform;
		if (entity.GetComponent<SpriteRendererComponent>().Visible)
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

	void Scene::SimulateEntityPhysics(Entity& entity, bool useTransformRef, glm::mat4 refTransform) {
		// get correct transform based on parent tree
		TransformComponent& tc = entity.GetComponent<TransformComponent>();
		glm::mat4 transform = tc.GetTransform();
		if (useTransformRef) transform = refTransform * transform;

		//calculate new transform and set it based on physics
		glm::vec3 translation, rotation, scale;
		Math::DecomposeTransform(transform, translation, rotation, scale);
		b2Body* body = entity.GetComponent<RigidBody2DComponent>().RuntimeBody;
		const auto& position = body->GetPosition();
		translation.x = position.x;
		translation.y = position.y;
		rotation.z = body->GetAngle();
		glm::mat4 newTransform = Math::ComposeTransform(translation, rotation, scale); // the world transform
		glm::mat4 updatedTransform = useTransformRef ? glm::inverse(refTransform) * newTransform : newTransform; // the component transform
		Math::DecomposeTransform(updatedTransform, translation, rotation, scale);
		tc.Translation.x = translation.x;
		tc.Translation.y = translation.y;
		tc.Rotation.z = rotation.z;
		//tc.Scale = scale;

		//for debugging, but could be extended to view hitboxes
		if (false) {
			Entity cameraEntity = Entity{ (entt::entity)(uint32_t)m_PrimaryCameraHandle, this };
			Camera* primaryCamera = &(cameraEntity.GetComponent<CameraComponent>().Camera);
			glm::mat4 cameraTransform = cameraEntity.GetComponent<TransformComponent>().GetTransform();
			Renderer2D::BeginScene(primaryCamera->GetProjection(), cameraTransform);
			Renderer2D::DrawQuad(newTransform, { 1, 1, 1, 1 }, (int)(uint32_t)entity);
			Renderer2D::EndScene();
		}

		//simulate children entities
		if (entity.HasComponent<ChildComponent>()) {
			std::vector<Entity> children = entity.GetComponent<ChildComponent>().Children;
			for (int i = 0; i < children.size(); i++) {
				if (children[i].HasComponent<RigidBody2DComponent>()) {
					bool useParentTransform = children[i].GetComponent<ParentComponent>().InheritAll || (!children[i].GetComponent<ParentComponent>().InheritAll && children[i].GetComponent<ParentComponent>().InheritTransform);
					SimulateEntityPhysics(children[i], useParentTransform, transform);
				}
			}
		}
	}

	glm::mat4 Scene::GetWorldTransform(Entity& entity) {
		if (entity.HasComponent<ParentComponent>()) {
			if (entity.GetComponent<ParentComponent>().InheritAll || entity.GetComponent<ParentComponent>().InheritTransform) {
				return GetWorldTransform(entity.GetComponent<ParentComponent>().Parent) * entity.GetComponent<TransformComponent>().GetTransform();
			}
		}
		return entity.GetComponent<TransformComponent>().GetTransform();
	}

	Scene::Scene() {
	}

	Scene::~Scene() {
	}

	void Scene::StartPhysics() {
		m_PhysicsWorld = new b2World({ 0.0f, -m_Gravity });
		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto e : view) {
			Entity entity = { e, this };
			glm::mat4 transform = GetWorldTransform(entity);
			glm::vec3 translation, rotation, scale;
			Math::DecomposeTransform(transform, translation, rotation, scale);
			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
			b2BodyDef bodyDef;
			bodyDef.type = (b2BodyType)rb2d.Type;
			bodyDef.position.Set(translation.x, translation.y);
			bodyDef.angle = rotation.z;
			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>()) {
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
				b2PolygonShape polygonShape;
				polygonShape.SetAsBox(scale.x * bc2d.Size.x, scale.y * bc2d.Size.y);
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &polygonShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				b2Fixture* fixture = body->CreateFixture(&fixtureDef);
				bc2d.RuntimeFixture = fixture;
			}

			if (entity.HasComponent<CircleCollider2DComponent>()) {
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				circleShape.m_radius = scale.x * cc2d.Radius;
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
				b2Fixture* fixture = body->CreateFixture(&fixtureDef);
				cc2d.RuntimeFixture = fixture;
			}
		}
	}

	void Scene::StepScene(uint32_t steps) {
		m_StepFrames += steps;
	}

	void Scene::OnRuntimeStart() {
		m_Running = true;
		StartPhysics();

		// Scripting
		if (ScriptEngine::IsInitialized()) {
			ScriptEngine::OnRuntimeStart(this);

			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view) {
				Entity entity = { e, this };
				ScriptEngine::CreateEntity(entity);
			}
		}

		m_Paused = false;
		m_StepFrames = 0;
	}

	void Scene::OnRuntimeStop() {
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
		if (ScriptEngine::IsInitialized())
			ScriptEngine::OnRuntimeStop();

		//stop audio
		auto view = m_Registry.view<AudioSourceComponent>();
		for (auto e : view) {
			Entity entity = { e, this };
			AudioSourceComponent& asc = entity.GetComponent<AudioSourceComponent>();
			AssetManager::AddAudio(asc.AudioFile);
			Ref<Audio> audio = AssetManager::GetAudio(asc.AudioFile);
			AudioCommand::Stop(*audio);
		}

		m_Paused = false;
		m_Running = false;
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
		CopyComponent<CircleRendererComponent>(entity, newEntity);
		CopyComponent<CameraComponent>(entity, newEntity);
		CopyComponent<ScriptComponent>(entity, newEntity);
		CopyComponent<ScriptManagerComponent>(entity, newEntity);
		CopyComponent<RigidBody2DComponent>(entity, newEntity);
		CopyComponent<BoxCollider2DComponent>(entity, newEntity);
		CopyComponent<CircleCollider2DComponent>(entity, newEntity);
		CopyComponent<AudioSourceComponent>(entity, newEntity);
		CopyComponent<AudioListenerComponent>(entity, newEntity);
	}

	static void UpdateAudioEntity(Entity entity) {
		AudioSourceComponent& asc = entity.GetComponent<AudioSourceComponent>();
		if (asc.AudioFile != "") {
			AssetManager::AddAudio(asc.AudioFile);
			Ref<Audio> audio = AssetManager::GetAudio(asc.AudioFile);
			glm::vec3 translation = entity.GetComponent<TransformComponent>().Translation;
			AudioCommand::Update(
				*audio,
				asc.Scale,
				asc.Pitch,
				asc.Gain,
				asc.Velocity,
				asc.Loop,
				translation
			);
			if (asc.State != AudioState::NONE) {
				switch (asc.State) {
				case AudioState::PLAY:
					AudioCommand::Play(
						*audio,
						asc.Scale,
						asc.Pitch,
						asc.Gain,
						asc.Velocity,
						asc.Loop,
						translation
					);
					break;
				case AudioState::STOP:
					AudioCommand::Stop(*audio);
					break;
				case AudioState::PAUSE:
					AudioCommand::Pause(*audio);
					break;
				case AudioState::REWIND:
					AudioCommand::Rewind(*audio);
					break;
				default:
					FL_CORE_ASSERT("UNKOWN AUDIO COMMAND TYPE");
					break;
				}
				asc.State = AudioState::NONE;
			}
		} // else pause them TODO
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

		// instantiate copied script if needed
		if (m_Running && newEntity.HasComponent<ScriptComponent>())
			ScriptEngine::CreateEntity(newEntity);

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

		// instantiate copied script if needed
		if (m_Running && newEntity.HasComponent<ScriptComponent>())
			ScriptEngine::CreateEntity(newEntity);

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
		// script manager
		//TODO

		// mono scripts
		if (ScriptEngine::IsInitialized()) {
			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view) {
				Entity entity = { e, this };
				ScriptEngine::UpdateEntity(entity, ts);
			}
		}
	}

	void Scene::RenderRuntime(glm::mat4 viewProjection) {
		Renderer2D::BeginScene(viewProjection);
		//quads
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group) {
				Entity drawEntity = Entity{ entity, this };
				if (!drawEntity.HasComponent<ParentComponent>())
					DrawEntitySprite(drawEntity);
				else if (!drawEntity.GetComponent<ParentComponent>().Parent.HasComponent<SpriteRendererComponent>()) {
					bool useParentTransform = drawEntity.GetComponent<ParentComponent>().InheritAll || (!drawEntity.GetComponent<ParentComponent>().InheritAll && drawEntity.GetComponent<ParentComponent>().InheritTransform);
					DrawEntitySprite(drawEntity, useParentTransform, GetWorldTransform(drawEntity.GetComponent<ParentComponent>().Parent));
				}
			}
		}

		//circles
		{
			auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : view) {
				auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);
				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)(uint32_t)entity);
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::UpdatePhysics(Timestep ts) {
		m_PhysicsWorld->Step(ts, m_PhysicsVelocityIterations, m_PhysicsPositionIterations);
		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto e : view) {
			Entity physEntity = Entity{ e, this };
			if (!physEntity.HasComponent<ParentComponent>())
				SimulateEntityPhysics(physEntity);
		}
	}

	void Scene::UpdateAudio() {
		auto view = m_Registry.view<AudioSourceComponent>();
		for (auto e : view) {
			Entity entity = { e, this };
			UpdateAudioEntity(entity);
		}

		if (m_PrimaryAudioListenerHandle >= 0) {
			Entity entity = { (entt::entity)((uint32_t)m_PrimaryAudioListenerHandle), this };
			TransformComponent tfc = entity.GetComponent<TransformComponent>();
			AudioListenerComponent alc = entity.GetComponent<AudioListenerComponent>();
			AudioCommand::UpdateListener(tfc.Translation, tfc.Rotation, alc.Velocity, alc.Gain);
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, glm::mat4 viewProjection) {
		RenderRuntime(viewProjection);
		UpdateAudio();
	}

	void Scene::OnUpdateRuntime(Timestep ts, glm::mat4 viewProjection) {
		if (!m_Paused || m_StepFrames > 0) {
			// Update Scripts
			UpdateScripts(ts);

			// Update Physics
			UpdatePhysics(ts);
		}

		// Update Audio
		UpdateAudio();

		// Render 2D Sprites
		RenderRuntime(viewProjection);

		if (m_StepFrames > 0) m_StepFrames--;
	}

	void Scene::OnUpdateRuntime(Timestep ts) {
		if (m_PrimaryCameraHandle >= 0) {
			Entity cameraEntity = Entity{ (entt::entity)(uint32_t)m_PrimaryCameraHandle, this };
			Camera* primaryCamera = &(cameraEntity.GetComponent<CameraComponent>().Camera);
			glm::mat4 cameraTransform = cameraEntity.GetComponent<TransformComponent>().GetTransform();
			OnUpdateRuntime(ts, primaryCamera->GetProjection() * glm::inverse(cameraTransform));
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

	Entity Scene::GetEntityFromID(uint32_t id) {
		return Entity{ (entt::entity)id, this };
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		static_assert(true);
	}
	
	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component) { }

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component) { }

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component) {
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<AudioSourceComponent>(Entity entity, AudioSourceComponent& component) {}

	template<>
	void Scene::OnComponentAdded<AudioListenerComponent>(Entity entity, AudioListenerComponent& component) {}
}