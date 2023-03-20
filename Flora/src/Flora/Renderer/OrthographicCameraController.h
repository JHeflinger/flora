#pragma once
#include "Flora/Renderer/OrthographicCamera.h"
#include "Flora/Core/Timestep.h"
#include "Flora/Events/ApplicationEvent.h"
#include "Flora/Events/MouseEvent.h"

namespace Flora {
	struct OrthographicCameraBounds {
		float Left, Right, Bottom, Top;
		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		void OnResize(float width, float height);
		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView(); }
		float GetZoomLevel() { return m_ZoomLevel; }
		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
	private:
		void CalculateView();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;
		OrthographicCameraBounds m_Bounds;
		bool m_Rotation;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;
		float m_CameraZoomSpeed = 0.35f;
	};
}