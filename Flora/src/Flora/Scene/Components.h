#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "Flora/Scene/SceneCamera.h"
#include "Flora/Scene/ScriptableEntity.h"
#include "Flora/Renderer/Texture.h"
#include "Flora/Utils/PhysicsUtils.h"

namespace Flora {
	enum class AudioState { NONE, PLAY, STOP, PAUSE, REWIND };

	struct TagComponent {
		std::string Tag;
		TagComponent() = default;
		TagComponent(const TagComponent& other) { Tag = other.Tag; }
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct TransformComponent {
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) {
			Translation = other.Translation;
			Rotation = other.Rotation;
			Scale = other.Scale;
		}
		TransformComponent(const glm::vec3& translation) : Translation(translation) {}
		glm::mat4 GetTransform() const {
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent {
		enum class SpriteType {SINGLE = 0, SUBTEXTURE = 1, ANIMATION = 2};
		bool Visible = true;
		glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
		SpriteType Type = SpriteType::SINGLE;
		float TilingFactor = 1.0f;
		int Rows = 1;
		int Columns = 1;
		int RowCoordinate = 1;
		int ColumnCoordinate = 1;
		float SubtextureWidth = 1.0f;
		float SubtextureHeight = 1.0f;
		int Frames = 1;
		int StartFrame = 1;
		int EndFrame = 1;
		int CurrentFrame = 1;
		int FPS = 1;
		int FrameCounter = 0;
		float Time = 0.0f;
		bool Paused = false;
		std::string Path = "NULL";
		std::string Filename = "None Selected";
		bool TextureInitialized = false;
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) {
			Visible = other.Visible;
			Color = other.Color;
			Type = other.Type;
			TilingFactor = other.TilingFactor;
			Rows = other.Rows;
			Columns = other.Columns;
			RowCoordinate = other.RowCoordinate;
			ColumnCoordinate = other.ColumnCoordinate;
			SubtextureWidth = other.SubtextureWidth;
			SubtextureHeight = other.SubtextureHeight;
			Frames = other.Frames;
			StartFrame = other.StartFrame;
			EndFrame = other.EndFrame;
			CurrentFrame = other.CurrentFrame;
			FPS = other.FPS;
			FrameCounter = other.FrameCounter;
			Path = other.Path;
			Filename = other.Filename;
			TextureInitialized = other.TextureInitialized;
		}
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
	};

	struct CircleRendererComponent {
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float Radius = 0.5f;
		float Thickness = 1.0f;
		float Fade = 0.005f;
		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent& other) {
			Color = other.Color;
			Radius = other.Radius;
			Thickness = other.Thickness;
			Fade = other.Fade;
		}
		CircleRendererComponent(const glm::vec4& color) : Color(color) {}
	};

	struct CameraComponent {
		SceneCamera Camera;
		bool FixedAspectRatio = false;
		bool ShowBorder = false; // TODO: move out of components since this is part of the debug overlay
		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) {
			Camera = other.Camera;
			FixedAspectRatio = other.FixedAspectRatio;
		}
	};

	struct ScriptComponent {
		std::string ClassName = "";

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent& other) {
			ClassName = other.ClassName;
		}
	};

	struct ScriptManagerComponent {
		std::vector<std::string> ClassNames;
		ScriptManagerComponent() = default;
		ScriptManagerComponent(const ScriptManagerComponent& other) {
			ClassNames = other.ClassNames;
		}
	};

	struct ChildComponent {
		std::vector<Entity> Children;
		bool HasChildren() { return Children.size() == 0; }
		void AddChild(Entity entity) { Children.emplace_back(entity); }
		void RemoveChild(Entity entity) {
			for (int i = 0; i < Children.size(); i++)
				if (Children[i] == entity)
					Children.erase(Children.begin() + i);
		}
		ChildComponent() = default;
	};

	struct ParentComponent {
		Entity Parent;
		bool InheritAll = true;
		bool InheritTransform = true;
		ParentComponent() = default;
		ParentComponent(const ParentComponent& other) {
			Parent = other.Parent;
			InheritAll = other.InheritAll;
			InheritTransform = other.InheritTransform;
		}
	};

	struct RigidBody2DComponent {
		enum class BodyType { STATIC = 0, KINEMATIC = 1, DYNAMIC = 2 };
		BodyType Type = BodyType::STATIC;
		bool FixedRotation = false;
		b2Body* RuntimeBody = nullptr;
		RigidBody2DComponent() = default;
		RigidBody2DComponent(const RigidBody2DComponent& other) {
			Type = other.Type;
			FixedRotation = other.FixedRotation;
		}
	};

	struct BoxCollider2DComponent {
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };
		float Density = 1.0f;
		float Friction = 0.5;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;
		void* RuntimeFixture = nullptr;
		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent& other) {
			Offset = other.Offset;
			Size = other.Size;
			Density = other.Density;
			Friction = other.Friction;
			Restitution = other.Restitution;
			RestitutionThreshold = other.RestitutionThreshold;
		}
	};

	struct CircleCollider2DComponent {
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;
		float Density = 1.0f;
		float Friction = 0.5;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;
		void* RuntimeFixture = nullptr;
		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent& other) {
			Offset = other.Offset;
			Radius = other.Radius;
			Density = other.Density;
			Friction = other.Friction;
			Restitution = other.Restitution;
			RestitutionThreshold = other.RestitutionThreshold;
		}
	};

	struct AudioSourceComponent {
		float Scale = 1;
		std::string AudioFile = "";
		bool Loop = false;
		float Pitch = 1.0f;
		float Gain = 1.0f;
		glm::vec3 Velocity = { 0.0f, 0.0f, 0.0f };
		AudioState State = AudioState::NONE;
		AudioSourceComponent() = default;
		AudioSourceComponent(const AudioSourceComponent& other) {
			Scale = other.Scale;
			AudioFile = other.AudioFile;
			Loop = other.Loop;
			Pitch = other.Pitch;
			Gain = other.Gain;
			Velocity = other.Velocity;
			State = other.State;
		}
	};

	struct AudioListenerComponent {
		glm::vec3 Velocity = { 0.0f, 0.0f, 0.0f };
		float Gain = 1.0f;
		AudioListenerComponent() = default;
		AudioListenerComponent(const AudioListenerComponent& other) {
			Velocity = other.Velocity;
			Gain = other.Gain;
		}
	};

	template<typename... Component>
	struct ComponentGroup{};

	using AllComponents =
		ComponentGroup<TagComponent, TransformComponent, SpriteRendererComponent,
		CircleRendererComponent, CameraComponent, ScriptComponent,
		ScriptManagerComponent, ChildComponent, ParentComponent, RigidBody2DComponent, 
		BoxCollider2DComponent, CircleCollider2DComponent, AudioSourceComponent,
		AudioListenerComponent>;
}
