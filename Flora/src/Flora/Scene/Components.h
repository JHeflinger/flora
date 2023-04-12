#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Flora/Scene/SceneCamera.h"
#include "Flora/Scene/ScriptableEntity.h"
#include "Flora/Renderer/Texture.h"
#include <glm/gtx/quaternion.hpp>

namespace Flora {
	struct TagComponent {
		std::string Tag;
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct TransformComponent {
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
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
		glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
		Ref<Texture2D> Texture;
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
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
	};

	struct CameraComponent {
		SceneCamera Camera;
		bool Primary = true; // TODO: think about moving to scene
		bool FixedAspectRatio = false;
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent {
		ScriptableEntity* Instance = nullptr;
		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind() {
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}
