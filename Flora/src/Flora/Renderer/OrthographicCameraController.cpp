#include "flpch.h"
#include "OrthographicCameraController.h"
#include "Flora/Core/Input.h"
#include "Flora/Core/KeyCodes.h"

namespace Flora {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Rotation(rotation),
		m_AspectRatio(aspectRatio) {

	}

	void OrthographicCameraController::OnUpdate(Timestep ts) {
		if (Input::IsKeyPressed(FL_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(FL_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		
		if (Input::IsKeyPressed(FL_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(FL_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		
		if (m_Rotation) {
			if (Input::IsKeyPressed(FL_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			else if (Input::IsKeyPressed(FL_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel * 2.0f;
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		EventDispacher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(FL_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(FL_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		m_ZoomLevel -= e.GetYOffset() * m_CameraZoomSpeed;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.02f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}