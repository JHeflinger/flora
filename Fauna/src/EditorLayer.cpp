#include "EditorLayer.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"
#include "Flora/Utils/Serializer.h"
#include "Flora/Utils/PlatformUtils.h"
#include "Flora/Math/Math.h"
#include "Utils/FileUtils.h"
#include "ImGuizmo.h"

namespace Flora {
	//temp, remove when projects are implemented
	extern const std::filesystem::path g_AssetPath;

	EditorLayer::EditorLayer()
		: Layer("Editor") {
		m_Panels["Stats"] = CreateScope<StatsPanel>();
		m_Panels["Scene Hierarchy"] = CreateScope<SceneHierarchyPanel>();
		m_Panels["Content Browser"] = CreateScope<ContentBrowserPanel>();
		m_Panels["Viewport"] = CreateScope<ViewportPanel>();
	}

	void EditorLayer::OnAttatch() {
		// Set up editor params
		ResetEditorParams();

		// Initialize panels
		InitializePanels();

		// Set panel context
		SetPanelContext();

		// Load editor settings
		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			Serializer::DeserializeScene(m_EditorParams->ActiveScene, sceneFilePath);
		} else Serializer::DeserializeEditor(m_EditorParams);
	}

	void EditorLayer::OnDetatch() {
		// Save editor settings
		Serializer::SerializeEditor(m_EditorParams);
		FL_CORE_INFO("Saved editor settings");
	}

	void EditorLayer::OnUpdate(Timestep ts) {
		// Prepare viewport
		GetSpecificPanel<ViewportPanel>("Viewport")->PreUpdate();

		// Update Panels
		UpdatePanels();

		// Update editor params
		UpdateEditorParams(ts);

		// Process viewport
		GetSpecificPanel<ViewportPanel>("Viewport")->PostUpdate();

		// Editor override events
		OnOverrideEvent();

		// Periodically serialize
		AutoSaveEditor(ts);
	}

	void EditorLayer::OnImGuiRender() {
		static bool dockspace_open = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen) {
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else {
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding) ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
		if (!opt_padding) ImGui::PopStyleVar();

		if (opt_fullscreen) ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 330.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = 32.0f;

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Scene")) {
				if (ImGui::MenuItem("New", "Ctrl+N")) {
					FileUtils::NewScene(m_EditorParams);
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O")) {
					FileUtils::OpenScene(m_EditorParams);
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S	")) {
					FileUtils::SaveSceneAs(m_EditorParams);
				}

				if (ImGui::MenuItem("Save", "Ctrl+S")) {
					FileUtils::SaveScene(m_EditorParams);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Application")) {
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		//===================put dockable imgui panels here=====================

		// Panels
		RenderImGuiPanels();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e) {
		m_EditorParams->EditorCamera.OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(FL_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(FL_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e) {
		// some shortcuts moved to OnOverrideEvent
		if (e.GetRepeatCount() > 0) return false;
		switch (e.GetKeyCode()) {
		case Key::Q:
			m_EditorParams->GizmoType = -1;
			break;
		case Key::W:
			m_EditorParams->GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case Key::E:
			m_EditorParams->GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case Key::R:
			m_EditorParams->GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		default:
			break;
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
		if (e.GetMouseButton() == Mouse::ButtonLeft &&
			GetSpecificPanel<ViewportPanel>("Viewport")->ViewportHovered() &&
			!ImGuizmo::IsUsing() &&
			!ImGuizmo::IsOver() &&
			!Input::IsKeyPressed(Key::Space))
			m_EditorParams->SelectedEntity = GetSpecificPanel<ViewportPanel>("Viewport")->GetHoveredEntity();
		return false;
	}

	void EditorLayer::ResetEditorParams() {
		m_EditorParams = CreateRef<EditorParams>();
		m_EditorParams->ActiveScene = CreateRef<Scene>();
		m_EditorParams->EditorCamera = EditorCamera(30.0f, 1.7778f, 0.1f, 1000.0f);
	}

	void EditorLayer::InitializePanels() {
		for (auto& panel : m_Panels) {
			panel.second->Initialize();
		}
	}

	void EditorLayer::SetPanelContext() {
		for (auto& panel : m_Panels) {
			panel.second->SetEditorContext(m_EditorParams);
		}
	}

	void EditorLayer::UpdatePanels() {
		for (auto& panel : m_Panels) {
			panel.second->OnUpdate();
		}
	}

	void EditorLayer::RenderImGuiPanels() {
		for (auto& panel : m_Panels) {
			panel.second->OnImGuiRender();
		}
	}

	void EditorLayer::UpdateEditorParams(Timestep ts) {
		if (!m_EditorParams->Resized) {
			glm::vec2 viewportSize = GetSpecificPanel<ViewportPanel>("Viewport")->GetViewportSize();
			m_EditorParams->ActiveScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_EditorParams->Resized = true;
		}

		m_EditorParams->EditorCamera.OnUpdate(ts);
		m_EditorParams->ActiveScene->OnUpdateEditor(ts, m_EditorParams->EditorCamera);
	}

	void EditorLayer::AutoSaveEditor(Timestep ts) {
		static float time = 0.0f;
		time += ts;
		if (time > 300) { // save settings every 5 minutes
			time = 0.0f;
			Serializer::SerializeEditor(m_EditorParams);
			FL_CORE_INFO("Automatically saved editor settings");
		}
	}

	void EditorLayer::OnOverrideEvent() {
		// Shortcuts
		if (m_OverrideEventReady) {
			m_OverrideEventReady = false;
			bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
			bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
			if (control && shift && Input::IsKeyPressed(Key::S)) {
				FileUtils::SaveSceneAs(m_EditorParams);
			} else if (control && Input::IsKeyPressed(Key::S)) {
				FileUtils::SaveScene(m_EditorParams);
			} else if (control && Input::IsKeyPressed(Key::N)) {
				FileUtils::NewScene(m_EditorParams);
			} else if (control && Input::IsKeyPressed(Key::O)) {
				FileUtils::OpenScene(m_EditorParams);
			} else {
				m_OverrideEventReady = true;
			}
		} else {
			m_OverrideEventReady = true;
		}
	}

}