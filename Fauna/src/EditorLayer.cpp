#include "EditorLayer.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"
#include "Flora/Utils/Serializer.h"
#include "Flora/Utils/PlatformUtils.h"
#include "Flora/Math/Math.h"
#include "ImGuizmo.h"

namespace Flora {
	//temp, remove when projects are implemented
	extern const std::filesystem::path g_AssetPath;

	EditorLayer::EditorLayer()
		: Layer("Editor") {
		m_Panels["Stats"] = CreateScope<StatsPanel>();
		//m_Panels["Scene Hierarchy"] = CreateScope<SceneHierarchyPanel>();
		//m_Panels["Content Browser"] = CreateScope<ContentBrowserPanel>();
		//m_Panels["Viewport"] = CreateScope<ViewportPanel>();
	}

	void EditorLayer::OnAttatch() {
		// Set up editor params
		ResetEditorParams();

		// Initialize panels
		InitializePanels();
		m_ViewportPanel.Initialize();

		// Set panel context
		SetPanelContext();
		m_ViewportPanel.SetEditorContext(m_EditorParams);

		// command like argument creation
		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			Serializer::DeserializeScene(m_EditorParams->ActiveScene, sceneFilePath);
		}

		//panel creation
		m_SceneHierarchyPanel.SetEditorContext(m_EditorParams);

		//editor camera
		m_EditorParams->EditorCamera = EditorCamera(30.0f, 1.7778f, 0.1f, 1000.0f);

		//update selected entity
		m_EditorParams->SelectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();

		//load editor settings
		Serializer::DeserializeEditor(m_EditorParams);
		//m_SceneHierarchyPanel.SetSelectedEntity(m_EditorParams->SelectedEntity); //<- does not work right now, implement when doing projects later
	}

	void EditorLayer::OnDetatch() {
		Serializer::SerializeEditor(m_EditorParams);
		FL_CORE_INFO("Saved editor settings");
	}

	void EditorLayer::OnUpdate(Timestep ts) {

		m_ViewportPanel.PreUpdate();

		m_ViewportPanel.OnUpdate();

		// Camera update
		m_EditorParams->EditorCamera.OnUpdate(ts);

		// Scene update
		m_EditorParams->ActiveScene->OnUpdateEditor(ts, m_EditorParams->EditorCamera);

		// Update Panels
		UpdatePanels();

		m_ViewportPanel.PostUpdate();

		// Editor override events
		OnOverrideEvent();

		// Periodically serialize
		static float time = 0.0f;
		time += ts;
		if (time > 300) { // save settings every 5 minutes
			time = 0.0f;
			Serializer::SerializeEditor(m_EditorParams);
			FL_CORE_INFO("Automatically saved editor settings");
		}

		// Temporary solution, update the selected entity in the hierarchy panel
		m_EditorParams->SelectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
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
					NewScene();
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O")) {
					OpenScene();
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S	")) {
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Save", "Ctrl+S")) {
					SaveScene();
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
		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();
		RenderImGuiPanels();
		m_ViewportPanel.OnImGuiRender();


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
		if (e.GetMouseButton() == Mouse::ButtonLeft && m_ViewportPanel.ViewportHovered() && !ImGuizmo::IsUsing() && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::Space))
			m_SceneHierarchyPanel.SetSelectedEntity(m_ViewportPanel.GetHoveredEntity());
		return false;
	}

	void EditorLayer::SaveScene() {
		std::string sceneFilepath = m_EditorParams->ActiveScene->GetSceneFilepath();
		if (sceneFilepath != "NULL") {
			Serializer::SerializeScene(m_EditorParams->ActiveScene, sceneFilepath);
		} else {
			SaveSceneAs();
		}
	}

	void EditorLayer::SaveSceneAs() {
		std::string filepath = FileDialogs::SaveFile("Flora Scene (*.flora)\0*.flora\0");
		if (!filepath.empty()) {
			Serializer::SerializeScene(m_EditorParams->ActiveScene, filepath);
		}
	}

	void EditorLayer::OpenScene() {
		std::string filepath = FileDialogs::OpenFile("Flora Scene (*.flora)\0*.flora\0");
		if (!filepath.empty()) {
			OpenScene(filepath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path) {
		NewScene();
		m_EditorParams->ActiveScene->SetSceneFilepath(path.string());
		Serializer::DeserializeScene(m_EditorParams->ActiveScene, path.string());
		Serializer::SerializeEditor(m_EditorParams);
	}

	void EditorLayer::NewScene() {
		glm::vec2 viewportSize = m_ViewportPanel.GetViewportSize();
		m_EditorParams->ActiveScene = CreateRef<Scene>();
		m_EditorParams->ActiveScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		m_SceneHierarchyPanel.SetEditorContext(m_EditorParams);
		m_EditorParams->ActiveScene->SetSceneFilepath("NULL");
	}

	void EditorLayer::ResetEditorParams() {
		m_EditorParams = CreateRef<EditorParams>();
		m_EditorParams->ActiveScene = CreateRef<Scene>();
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

	void EditorLayer::OnOverrideEvent() {
		// Shortcuts
		if (m_OverrideEventReady) {
			m_OverrideEventReady = false;
			bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
			bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
			if (control && shift && Input::IsKeyPressed(Key::S)) {
				SaveSceneAs();
			} else if (control && Input::IsKeyPressed(Key::S)) {
				SaveScene();
			} else if (control && Input::IsKeyPressed(Key::N)) {
				NewScene();
			} else if (control && Input::IsKeyPressed(Key::O)) {
				OpenScene();
			} else {
				m_OverrideEventReady = true;
			}
		} else {
			m_OverrideEventReady = true;
		}
	}
}