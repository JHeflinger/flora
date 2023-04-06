#pragma once
#include "Camera.h"
#include "Flora/Core/Timestep.h"
#include "Flora/Events/Event.h"
#include "Flora/Events/MouseEvent.h"
#include <glm/glm.hpp>

namespace Flora {
	class EditorCamera : public Camera {
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }
		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;
		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
		std::string GetCameraTypeString();
		void SetCameraTypeWithString(const std::string cameraType);
		void SetPosition(glm::vec3 position) { m_Position = position; }
		void SetPitch(float pitch) { m_Pitch = pitch; }
		void SetYaw(float yaw) { m_Yaw = yaw; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; }
		float GetOrthographicSize() const { return m_OrthographicSize; }
		glm::vec3 GetFocalPoint() const { return m_FocalPoint; }
		void SetFocalPoint(glm::vec3 focalPoint) { m_FocalPoint = focalPoint; }
	private:
		void UpdateProjection();
		void UpdateView();
		bool OnMouseScroll(MouseScrolledEvent& e);
		void MousePan(const glm::vec2& delta);
		void MouseRotate(const glm::vec2& delta);
		void MouseZoom(float delta);
		glm::vec3 CalculatePosition() const;
		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;
		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };
		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;
		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
		ProjectionType m_ProjectionType = ProjectionType::Perspective;
		float m_OrthographicSize = 10.0f;
		//change later to something modular/dynamic
		float m_OrthographicNear = -1000.0f;
		float m_OrthographicFar = 1000.0f;
		//temp
		bool m_ToggleEnable = true;
	};
}