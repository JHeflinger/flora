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
	}

	void EditorLayer::OnAttatch() {
		//framebuffer creation
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		//scene creation
		m_EditorParams.ActiveScene = CreateRef<Scene>();

		// command like argument creation
		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			Serializer::DeserializeScene(m_EditorParams.ActiveScene, sceneFilePath);
		}

		//panel creation
		m_SceneHierarchyPanel.SetContext(m_EditorParams.ActiveScene);

		//editor camera
		m_EditorParams.EditorCamera = EditorCamera(30.0f, 1.7778f, 0.1f, 1000.0f);

		//load editor settings
		Serializer::DeserializeEditor(m_EditorParams);
		//m_SceneHierarchyPanel.SetSelectedEntity(m_EditorParams.SelectedEntity); <- does not work right now, implement when doing projects later
	}

	void EditorLayer::OnDetatch() {
		Serializer::SerializeEditor(m_EditorParams);
		FL_CORE_INFO("Saved editor settings");
	}

	void EditorLayer::OnUpdate(Timestep ts) {
		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorParams.EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorParams.ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Camera update
		m_EditorParams.EditorCamera.OnUpdate(ts);

		// renderer setup, move later
		Renderer2D::ResetStats();	
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// clear entity ID attachment to -1
		m_Framebuffer->ClearAttachment(1, -1);

		// Scene update
		m_EditorParams.ActiveScene->OnUpdateEditor(ts, m_EditorParams.EditorCamera);

		// Temp
		{
			auto [mx, my] = ImGui::GetMousePos();
			mx -= m_ViewportBounds[0].x;
			my -= m_ViewportBounds[0].y;
			glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
			my = viewportSize.y - my;
			int mouseX = (int)mx;
			int mouseY = (int)my;
			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
				int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
				m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_EditorParams.ActiveScene.get());
			}
		}

		// Editor override events
		OnOverrideEvent();

		// renderer "unsetup", move later
		m_Framebuffer->Unbind();

		// Periodically serialize
		static float time = 0.0f;
		time += ts;
		if (time > 300) { // save settings every 5 minutes
			time = 0.0f;
			Serializer::SerializeEditor(m_EditorParams);
			FL_CORE_INFO("Automatically saved editor settings");
		}

		// Temporary solution, update the selected entity in the hierarchy panel
		m_EditorParams.SelectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
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
		m_StatsPanel.OnImGuiRender();

		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x , viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x , viewportMaxRegion.y + viewportOffset.y };
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		m_EditorParams.ActiveScene->SetViewportHovered(m_ViewportHovered); // temporary solution
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// Drag scenes into viewport capability
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_AssetPath) / path);
			}
			ImGui::EndDragDropTarget();
		}

		// Viewport Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_EditorParams.GizmoType != -1) {
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
			
			// Editor Camera
			const glm::mat4& cameraProjection = m_EditorParams.EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorParams.EditorCamera.GetViewMatrix();

			// Runtime Camera
			/*auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			const glm::mat4& cameraProjection = camera.GetProjection();
			glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());*/

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;
			if (m_EditorParams.GizmoType == ImGuizmo::OPERATION::ROTATE) snapValue = 45.0f;
			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_EditorParams.GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing()) {
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);
				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = glm::vec3(transform[3]);
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		// End Viewport
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e) {
		m_EditorParams.EditorCamera.OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(FL_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(FL_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e) {
		// some shortcuts moved to OnOverrideEvent
		if (e.GetRepeatCount() > 0) return false;
		switch (e.GetKeyCode()) {
		case Key::Q:
			m_EditorParams.GizmoType = -1;
			break;
		case Key::W:
			m_EditorParams.GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case Key::E:
			m_EditorParams.GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case Key::R:
			m_EditorParams.GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		default:
			break;
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e) {
		if (e.GetMouseButton() == Mouse::ButtonLeft && m_ViewportHovered && !ImGuizmo::IsUsing() && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::Space))
			m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		return false;
	}

	void EditorLayer::SaveScene() {
		std::string sceneFilepath = m_EditorParams.ActiveScene->GetSceneFilepath();
		if (sceneFilepath != "NULL") {
			Serializer::SerializeScene(m_EditorParams.ActiveScene, sceneFilepath);
		} else {
			SaveSceneAs();
		}
	}

	void EditorLayer::SaveSceneAs() {
		std::string filepath = FileDialogs::SaveFile("Flora Scene (*.flora)\0*.flora\0");
		if (!filepath.empty()) {
			Serializer::SerializeScene(m_EditorParams.ActiveScene, filepath);
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
		m_EditorParams.ActiveScene->SetSceneFilepath(path.string());
		Serializer::DeserializeScene(m_EditorParams.ActiveScene, path.string());
		Serializer::SerializeEditor(m_EditorParams);
	}

	void EditorLayer::NewScene() {
		m_EditorParams.ActiveScene = CreateRef<Scene>();
		m_EditorParams.ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_EditorParams.ActiveScene);
		m_EditorParams.ActiveScene->SetSceneFilepath("NULL");
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