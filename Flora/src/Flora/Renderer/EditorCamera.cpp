#include "flpch.h"
#include "Flora/Renderer/EditorCamera.h"
#include "Flora/Core/Input.h"
#include "Flora/Core/KeyCodes.h"
#include "Flora/Core/MouseCodes.h"
#include <glfw/glfw3.h>
#include <glm/gtx/quaternion.hpp>

namespace Flora {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)) {
		UpdateView();
	}

	void EditorCamera::UpdateProjection() {
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		if (m_ProjectionType == ProjectionType::Perspective) {
			m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
		}
		else {
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(orthoLeft, orthoRight,
				orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}
	}

	void EditorCamera::UpdateView() {
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	std::pair<float, float> EditorCamera::PanSpeed() const {
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const {
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const {
		float distance = m_Distance * 0.5f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera::OnUpdate(Timestep ts, bool updateControl) {
		if (Input::IsKeyPressed(Key::Space) && updateControl) {
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MouseZoom(delta.y);
			
			if (Input::IsKeyPressed(Key::GraveAccent)) { // reset camera position
				ResetCamera();
			}

			if (Input::IsKeyPressed(Key::LeftAlt)) { // toggle camera type
				if (m_ToggleEnable) {
					if (m_ProjectionType == ProjectionType::Perspective)
						m_ProjectionType = ProjectionType::Orthographic;
					else
						m_ProjectionType = ProjectionType::Perspective;
					m_ToggleEnable = false;
					UpdateProjection();
				}
			}
			else {
				m_ToggleEnable = true;
			}
		}
		UpdateView();
	}

	void EditorCamera::ResetCamera() {
		m_Distance = 10.0f;
		m_Pitch = 0.0f, m_Yaw = 0.0f;
		m_Position = { 0.0f, 0.0f, 0.0f };
		m_FocalPoint = { 0.0f, 0.0f, 0.0f };
	}

	void EditorCamera::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(FL_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e) {
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta) {
		auto [xSpeed, ySpeed] = PanSpeed();
		if (m_ProjectionType == ProjectionType::Orthographic) {
			xSpeed = 1;
			ySpeed = 1;
		}
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta) {
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta) {
		if (m_ProjectionType == ProjectionType::Perspective) {
			m_Distance -= delta * ZoomSpeed();
			if (m_Distance < 1.0f) {
				m_FocalPoint += GetForwardDirection();
				m_Distance = 1.0f;
			}
		} else {
			m_OrthographicSize -= delta* ZoomSpeed();
			UpdateProjection();
		}
	}

	void EditorCamera::SetCameraTypeWithString(const std::string cameraType) {
		if (cameraType == "Orthographic")
			m_ProjectionType = ProjectionType::Orthographic;
		else
			m_ProjectionType = ProjectionType::Perspective;
		UpdateProjection();
	}

	std::string EditorCamera::GetCameraTypeString() {
		if (m_ProjectionType == ProjectionType::Orthographic)
			return "Orthographic";
		return "Perspective";
	}

	glm::vec3 EditorCamera::GetUpDirection() const {
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRightDirection() const {
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForwardDirection() const {
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCamera::CalculatePosition() const {
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

	glm::quat EditorCamera::GetOrientation() const {
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

}