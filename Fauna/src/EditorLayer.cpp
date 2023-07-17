#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"
#include "Flora/Utils/Serializer.h"
#include "Flora/Utils/PlatformUtils.h"
#include "Flora/Math/Math.h"
#include "Utils/FileUtils.h"
#include "ImGuizmo.h"
#include "Flora/Audio/AudioCommand.h"
#include "Flora/Scripting/ScriptEngine.h"

namespace Flora {
	EditorLayer::EditorLayer()
		: Layer("Editor") {
		m_Panels["Stats"] = CreateScope<StatsPanel>();
		m_Panels["Scene Hierarchy"] = CreateScope<SceneHierarchyPanel>();
		m_Panels["Properties"] = CreateScope<PropertiesPanel>();
		m_Panels["Content Browser"] = CreateScope<ContentBrowserPanel>();
		m_Panels["Viewport"] = CreateScope<ViewportPanel>();
		m_Panels["Console"] = CreateScope<ConsolePanel>();
		m_Panels["Physics"] = CreateScope<PhysicsPanel>();
	}

	void EditorLayer::OnAttatch() {
		// Set up editor params
		ResetEditorParams();

		// Initialize editor
		InitializeEditor();

		// Set panel context
		SetPanelContext();

		// Initialize panels
		InitializePanels();
	}

	void EditorLayer::OnDetatch() {
	}

	void EditorLayer::OnUpdate(Timestep ts) {
		// Prepare viewport
		GetSpecificPanel<ViewportPanel>("Viewport")->PreUpdate();

		// Update Panels
		UpdatePanels(ts);

		// Update editor params
		UpdateEditor(ts);

		// Render debug overlay
		RenderDebugOverlay();

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
		style.WindowMinSize.x = 380.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = 32.0f;

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Scene")) {
				if (ImGui::MenuItem("New", "Ctrl+N")) {
					PromptSave(SavePromptType::NEW);
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O")) {
					PromptSave(SavePromptType::OPEN);
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
					FileUtils::SaveSceneAs(m_EditorParams);
					m_EditorParams->Time = 0;
				}

				if (ImGui::MenuItem("Save", "Ctrl+S")) {
					FileUtils::SaveScene(m_EditorParams);
					m_EditorParams->Time = 0;
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Application")) {
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows")) {
				for (auto& panel : m_Panels) {
					ImGui::MenuItem(panel.first.c_str(), NULL, &(panel.second->m_Enabled));
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Editor")) {
				if (m_EditorParams->EditorCamera.GetCameraTypeString() == "Orthographic") {
					if (ImGui::MenuItem("Switch to perspective", "Space+Alt")) {
						m_EditorParams->EditorCamera.SetCameraTypeWithString("Perspective");
					}
				}
				else {
					if (ImGui::MenuItem("Switch to orthographic", "Space+Alt")) {
						m_EditorParams->EditorCamera.SetCameraTypeWithString("Orthographic");
					}
				}

				if (ImGui::MenuItem("Reset Editor Camera", "Space+`"))
					m_EditorParams->EditorCamera.ResetCamera();

				ImGui::MenuItem("Bind Editor Camera", "Space+Ctrl", m_EditorParams->EditorCamera.GetCameraBound());

				ImGui::EndMenu();
			}

			ImGui::SameLine(ImGui::GetWindowSize().x);
			std::string lastSaved = "Last Saved " + GetLastSavedString() + " ago";
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::CalcTextSize(lastSaved.c_str()).x - 10);
			ImGui::PushStyleColor(ImGuiCol_Text, {0.6, 0.6, 0.6, 1});
			ImGui::Text(lastSaved.c_str());
			ImGui::PopStyleColor();

			ImGui::SameLine((ImGui::GetWindowSize().x) / 2);
			std::string viewportName = m_EditorParams->ActiveScene->GetSceneName();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::CalcTextSize(viewportName.c_str()).x / 2);
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
			static bool s_RenameScene = false;
			if (!s_RenameScene) {
				if (ImGui::Button(viewportName.c_str())) {
					s_RenameScene = true;
				}
			} else {
				static char newSceneName[256] = "";
				strncpy(newSceneName, viewportName.c_str(), 256);
				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
				ImGui::SetNextItemWidth(500);
				s_RenameScene = !(ImGui::InputText("##Rename", newSceneName, 256, ImGuiInputTextFlags_EnterReturnsTrue));
				ImGui::PopStyleColor();
				if (!s_RenameScene)
					m_EditorParams->ActiveScene->SetSceneName(std::string(newSceneName));
			}
			ImGui::PopStyleColor(3);

			ImGui::EndMenuBar();
		}

		//===================put dockable imgui panels here=====================

		// Panels
		RenderImGuiPanels();

		// Render UI Bar
		RenderUIBar();

		// Prompt Save
		RenderSavePrompt();

		// Prompt Project
		RenderProjectPrompt();

		// Prompt Errors
		RenderErrorPrompt();

		ImGui::End();
	}

	void EditorLayer::RenderUIBar() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 2 });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0, 0 });
		ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		float size = ImGui::GetWindowHeight() - 4.0f;
		ImGui::SameLine((ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5f));
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
		if (m_EditorParams->SceneState == SceneState::EDIT) {
			if (ImGui::ImageButton((ImTextureID)m_IconPlay->GetRendererID(), { size, size }))
				OnScenePlay();
		} else {
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - size - 16);
			if (ImGui::ImageButton((ImTextureID)m_IconStop->GetRendererID(), { size, size })) {
				OnSceneStop();
			}
			ImGui::SameLine();
			Ref<Texture2D> icon = m_EditorParams->ActiveScene->IsScenePaused() ? m_IconPlay : m_IconPause;
			if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { size, size })) {
				OnScenePause();
			}
			ImGui::SameLine();
			if (ImGui::ImageButton((ImTextureID)m_IconStep->GetRendererID(), { size, size })) {
				OnSceneStep();
			}
		}
		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(2);
		ImGui::End();
	}

	void EditorLayer::OnScenePlay() {
		FileUtils::SaveTempScene(m_EditorParams);
		m_EditorParams->SceneState = SceneState::PLAY;
		m_EditorParams->ActiveScene->OnRuntimeStart();

		// warnings
		if (!m_EditorParams->ActiveScene->GetPrimaryCamera()) FL_WARN("No primary camera selected");
	}

	void EditorLayer::OnScenePause() {
		if (m_EditorParams->ActiveScene->IsScenePaused())
			m_EditorParams->ActiveScene->SetScenePaused(false);
		else
			m_EditorParams->ActiveScene->SetScenePaused(true);
	}

	void EditorLayer::OnSceneStep() {
		m_EditorParams->ActiveScene->StepScene(1);
	}

	void EditorLayer::OnSceneStop() {
		m_EditorParams->ActiveScene->OnRuntimeStop();
		FileUtils::OpenTempScene(m_EditorParams);
		m_EditorParams->SceneState = SceneState::EDIT;
	}

	void EditorLayer::ProcessWindowClose() {
		// Save editor settings
		FileUtils::SaveEditor(m_EditorParams);
		FL_CORE_INFO("Saved editor settings");

		// Prompt save
		if (m_EditorParams->SceneState == SceneState::PLAY)
			OnSceneStop();
		PromptSave(SavePromptType::FINAL);
	}

	void EditorLayer::OnEvent(Event& e) {
		if (m_EditorParams->HoveredPanel == Panels::VIEWPORT)
			m_EditorParams->EditorCamera.OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(FL_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(FL_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	void EditorLayer::RenderErrorPrompt() {
		if (m_EditorParams->Error != "") {
			ImGui::OpenPopup("ERROR"); 
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			if (ImGui::BeginPopupModal("ERROR", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::SetItemDefaultFocus();
				std::string message = m_EditorParams->Error.substr(10) + "\n\n";
				ImGui::Text(message.c_str());
				ImGui::Separator();
				ImGui::Dummy({ 0, 3 });
				if (ImGui::Button("OK", { 60, 25 })) {
					m_EditorParams->Error = "";
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
	}

	void EditorLayer::RenderSavePrompt() {
		std::string filepath = "";
		if (GetSpecificPanel<ViewportPanel>("Viewport")->IsRequestingOpenScene())
			filepath = GetSpecificPanel<ViewportPanel>("Viewport")->GetRequestedStringPath();
		else if (GetSpecificPanel<ContentBrowserPanel>("Content Browser")->IsRequestingOpenScene())
			filepath = GetSpecificPanel<ContentBrowserPanel>("Content Browser")->GetRequestedStringPath();
		if (m_SavePromptType != SavePromptType::NONE && !Serializer::IsSceneSaved(m_EditorParams->ActiveScene)) {
			ImGui::OpenPopup("WARNING");
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			if (ImGui::BeginPopupModal("WARNING", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
				ImGui::SetItemDefaultFocus();
				ImGui::Text("You have not saved this scene! Would you like to save first?\n\n");
				ImGui::Separator();
				ImGui::Dummy({ 0, 3 });
				if (ImGui::Button("YES", { 60, 25 })) {
					switch (m_SavePromptType) {
					case SavePromptType::NORM:
						FileUtils::SaveScene(m_EditorParams);
						m_EditorParams->Time = 0;
						m_SavePromptType = SavePromptType::NONE;
						break;
					case SavePromptType::FINAL:
						FileUtils::SaveScene(m_EditorParams);
						m_SavePromptType = SavePromptType::NONE;
						m_ReadyToClose = true;
						break;
					case SavePromptType::OPEN:
						FileUtils::SaveScene(m_EditorParams);
						m_SavePromptType = SavePromptType::NONE;
						FileUtils::OpenScene(m_EditorParams);
						break;
					case SavePromptType::NEW:
						FileUtils::SaveScene(m_EditorParams);
						m_SavePromptType = SavePromptType::NONE;
						FileUtils::NewScene(m_EditorParams);
						break;
					case SavePromptType::OPENPATH:
						FileUtils::SaveScene(m_EditorParams);
						m_SavePromptType = SavePromptType::NONE;
						FileUtils::OpenScene(m_EditorParams, filepath);
						if (GetSpecificPanel<ViewportPanel>("Viewport")->IsRequestingOpenScene())
							GetSpecificPanel<ViewportPanel>("Viewport")->ResolveOpenSceneRequest();
						else if (GetSpecificPanel<ContentBrowserPanel>("Content Browser")->IsRequestingOpenScene())
							GetSpecificPanel<ContentBrowserPanel>("Content Browser")->ResolveOpenSceneRequest();
						break;
					}					
					ImGui::CloseCurrentPopup();
					m_EditorParams->Time = 0;
				}
				ImGui::SameLine();
				if (ImGui::Button("NO", { 60, 25 })) {
					switch (m_SavePromptType) {
					case SavePromptType::NORM:
						m_SavePromptType = SavePromptType::NONE;
						break;
					case SavePromptType::FINAL:
						m_SavePromptType = SavePromptType::NONE;
						m_ReadyToClose = true;
						break;
					case SavePromptType::OPEN:
						m_SavePromptType = SavePromptType::NONE;
						FileUtils::OpenScene(m_EditorParams);
						break;
					case SavePromptType::NEW:
						m_SavePromptType = SavePromptType::NONE;
						FileUtils::NewScene(m_EditorParams);
						m_EditorParams->Time = 0;
						break;
					case SavePromptType::OPENPATH:
						m_SavePromptType = SavePromptType::NONE;
						FileUtils::OpenScene(m_EditorParams, filepath);
						if (GetSpecificPanel<ViewportPanel>("Viewport")->IsRequestingOpenScene())
							GetSpecificPanel<ViewportPanel>("Viewport")->ResolveOpenSceneRequest();
						else if (GetSpecificPanel<ContentBrowserPanel>("Content Browser")->IsRequestingOpenScene())
							GetSpecificPanel<ContentBrowserPanel>("Content Browser")->ResolveOpenSceneRequest();
						break;
					}
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		} else {
			switch (m_SavePromptType) {
			case SavePromptType::FINAL:
				m_ReadyToClose = true;
				break;
			case SavePromptType::OPENPATH:
				FileUtils::OpenScene(m_EditorParams, filepath);
				if (GetSpecificPanel<ViewportPanel>("Viewport")->IsRequestingOpenScene())
					GetSpecificPanel<ViewportPanel>("Viewport")->ResolveOpenSceneRequest();
				else if (GetSpecificPanel<ContentBrowserPanel>("Content Browser")->IsRequestingOpenScene())
					GetSpecificPanel<ContentBrowserPanel>("Content Browser")->ResolveOpenSceneRequest();
				break;
			case SavePromptType::OPEN:
				FileUtils::OpenScene(m_EditorParams);
				break;
			case SavePromptType::NEW:
				FileUtils::NewScene(m_EditorParams);
				m_EditorParams->Time = 0;
				break;
			}
			m_SavePromptType = SavePromptType::NONE;
		}
	}

	void EditorLayer::RenderProjectPrompt() {
		bool firstProject = m_EditorParams->ProjectFilepath == "";
		if (firstProject) {
			ImGui::OpenPopup("New Fauna Project");
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			if (ImGui::BeginPopupModal("New Fauna Project", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
				static char filepath_buffer[1024];
				static char name_buffer[1024];
				float text_input_width = 300.0f;
				ImGui::SetItemDefaultFocus();
				ImGui::Text("Project Name");
				ImGui::Dummy({ 0, 2 });
				ImGui::SetNextItemWidth(text_input_width);
				ImGui::InputText("##ProjectName", name_buffer, sizeof(name_buffer));
				ImGui::Dummy({ 0, 10 });
				ImGui::Text("Project Directory");
				ImGui::Dummy({ 0, 2 });
				if (ImGui::Button("Browse", {60, 25})) {
					std::string filepath = FileDialogs::OpenFolder();
					strcpy(filepath_buffer, filepath.c_str());
				}
				ImGui::SameLine();
				ImGui::SetNextItemWidth(text_input_width - 67);
				ImGui::InputText("##ProjectDir", filepath_buffer, sizeof(filepath_buffer));
				ImGui::Dummy({ 0, 7 });
				ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 53);
				bool isvalid = std::strlen(name_buffer) != 0;
				if (isvalid)
					isvalid = std::filesystem::exists(filepath_buffer) && std::filesystem::is_directory(filepath_buffer);
				if (!isvalid) {
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				}
				if (ImGui::Button("OK", { 60, 25 })) {
					Project::GenerateProjectDirectory(std::string(name_buffer), std::filesystem::path(filepath_buffer));
					m_EditorParams->ProjectFilepath = std::string(filepath_buffer) + std::string(name_buffer) + ".flproj";
					m_EditorParams->Project->GetConfig().AssetDirectory = std::filesystem::path(filepath_buffer) / "Assets";
					m_EditorParams->Project->GetConfig().Name = std::string(name_buffer);
					m_EditorParams->Project->SaveActive(std::filesystem::path(filepath_buffer) / (std::string(name_buffer) + ".flproj"));
					ImGui::CloseCurrentPopup();
				}
				if (!isvalid) {
					ImGui::PopItemFlag();
					ImGui::PopStyleVar();
				}
				ImGui::EndPopup();
			}
		}
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
			m_EditorParams->HoveredPanel == Panels::VIEWPORT &&
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

	void EditorLayer::InitializeEditor() {
		// Load editor settings
		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.Count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			Serializer::DeserializeScene(m_EditorParams->ActiveScene, sceneFilePath);
		}
		else FileUtils::LoadEditor(m_EditorParams);

		// Close panels
		for (int i = 0; i < m_EditorParams->ClosedPanels.size(); i++)
			m_Panels[m_EditorParams->ClosedPanels[i]]->m_Enabled = false;

		// Set icons
		m_IconPlay = Texture2D::Create("Resources/Icons/Editor/PlayButton.png");
		m_IconStop = Texture2D::Create("Resources/Icons/Editor/StopButton.png");
		m_IconPause = Texture2D::Create("Resources/Icons/Editor/PauseButton.png");
		m_IconStep = Texture2D::Create("Resources/Icons/Editor/StepButton.png");
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

	std::string EditorLayer::GetLastSavedString() {
		float time = m_EditorParams->Time;
		std::stringstream ss;
		if (time < 60.0)
			ss << (int)time << " seconds";
		else if (time / 60.0f < 60.0)
			ss << (int)(time / 60.0f) << " minutes";
		else ss << (int)(time / 3600.0f) << " hours";
		return ss.str();
	}

	void EditorLayer::UpdatePanels(Timestep ts) {
		for (auto& panel : m_Panels) {
			if (panel.second->m_Enabled)
				panel.second->OnUpdate();
		}

		// process panel extras
		if (GetSpecificPanel<ViewportPanel>("Viewport")->IsRequestingOpenScene() || GetSpecificPanel<ContentBrowserPanel>("Content Browser")->IsRequestingOpenScene())
			PromptSave(SavePromptType::OPENPATH);

		GetSpecificPanel<StatsPanel>("Stats")->UpdateFrametime((float)ts);
	}

	void EditorLayer::RenderImGuiPanels() {
		for (auto& panel : m_Panels) {
			if (panel.second->m_Enabled)
				panel.second->OnImGuiRender();
		}
	}

	void EditorLayer::RenderDebugOverlay() {
		switch (m_EditorParams->SceneState) {
			case SceneState::EDIT:
				Renderer2D::BeginScene(m_EditorParams->EditorCamera.GetViewProjection());

				//highlight selected entity
				if (m_EditorParams->SelectedEntity) {
					glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
					glm::mat4 transform = m_EditorParams->SelectedEntity.GetComponent<TransformComponent>().GetTransform();
					if (m_EditorParams->SelectedEntity.HasComponent<SpriteRendererComponent>()) {
						glm::mat4 p1_mat = glm::translate(transform, { 1,  1, 0 });
						glm::mat4 p2_mat = glm::translate(transform, { 1, -1, 0 });
						glm::mat4 p3_mat = glm::translate(transform, { -1,  1, 0 });
						glm::mat4 p4_mat = glm::translate(transform, { -1, -1, 0 });
						glm::vec3 p1_coord, p2_coord, p3_coord, p4_coord;
						glm::vec3 rotation, scale;
						Math::DecomposeTransform(p1_mat, p1_coord, rotation, scale);
						Math::DecomposeTransform(p2_mat, p2_coord, rotation, scale);
						Math::DecomposeTransform(p3_mat, p3_coord, rotation, scale);
						Math::DecomposeTransform(p4_mat, p4_coord, rotation, scale);
						Renderer2D::DrawLine(p1_coord, p2_coord, color);
						Renderer2D::DrawLine(p4_coord, p2_coord, color);
						Renderer2D::DrawLine(p1_coord, p3_coord, color);
						Renderer2D::DrawLine(p3_coord, p4_coord, color);
					} else if (m_EditorParams->SelectedEntity.HasComponent<CircleRendererComponent>()) {
						float thickness = 0.05f;
						transform = glm::scale(transform, { 1.0f + thickness*2.0f, 1.0f + thickness*2.0f, 1.0f });
						Renderer2D::DrawCircle(transform, color, thickness);
					}
				}

				break;
			case SceneState::PLAY:
				if (*(m_EditorParams->EditorCamera.GetCameraBound())) {
					Renderer2D::BeginScene(m_EditorParams->EditorCamera.GetViewProjection());
				} else if (m_EditorParams->ActiveScene->GetPrimaryCamera() >= 0) {
					Entity primaryCameraEntity = m_EditorParams->ActiveScene->GetEntityFromID((uint32_t)m_EditorParams->ActiveScene->GetPrimaryCamera());
					Camera* primaryCamera = &(primaryCameraEntity.GetComponent<CameraComponent>().Camera);
					glm::mat4 cameraTransform = primaryCameraEntity.GetComponent<TransformComponent>().GetTransform();
					Renderer2D::BeginScene(primaryCamera->GetProjection() * glm::inverse(cameraTransform));
				}
				break;
			default:
				FL_CORE_ERROR("Invalid scene state detected!");
				return;
		}

		if (m_EditorParams->VisibleColliders) {
			// Circle collider visualizer
			{
				auto view = m_EditorParams->ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view) {
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);
					glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.0f);
					float thickness = 0.05f; // TODO: make this look constant despite camera depth
					glm::vec3 scale = tc.Scale * glm::vec3((cc2d.Radius) * 2.0f + thickness);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), thickness);
				}
			}

			// Box collider visualizer
			{
				auto view = m_EditorParams->ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view) {
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);
					glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.0f);
					glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
				}
			}
		}

		// camera borders
		auto view = m_EditorParams->ActiveScene->GetAllEntitiesWith<TransformComponent, CameraComponent>();
		for (auto entity : view) {
			auto [tc, cc] = view.get<TransformComponent, CameraComponent>(entity);
			if (cc.ShowBorder) {
				glm::vec4 color = { 1, 1, 1, 0.7f };
				if (cc.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
					float size = cc.Camera.GetOrthographicSize();
					float nearBorder = cc.Camera.GetOrthographicNearClip() * -1.0f;
					float farBorder =  (-1.0f * cc.Camera.GetOrthographicFarClip()) - nearBorder;
					glm::mat4 transform = tc.GetTransform();
					glm::mat4 p1_mat = glm::translate(transform, { size,  size, nearBorder });
					glm::mat4 p2_mat = glm::translate(transform, { size, -size, nearBorder });
					glm::mat4 p3_mat = glm::translate(transform, { -size,  size, nearBorder });
					glm::mat4 p4_mat = glm::translate(transform, { -size, -size, nearBorder });
					glm::mat4 p5_mat = glm::translate(p1_mat, { 0, 0, farBorder });
					glm::mat4 p6_mat = glm::translate(p2_mat, { 0, 0, farBorder });
					glm::mat4 p7_mat = glm::translate(p3_mat, { 0, 0, farBorder });
					glm::mat4 p8_mat = glm::translate(p4_mat, { 0, 0, farBorder });
					glm::vec3 p1_coord, p2_coord, p3_coord, p4_coord, p5_coord, p6_coord, p7_coord, p8_coord;
					glm::vec3 rotation, scale;
					Math::DecomposeTransform(p1_mat, p1_coord, rotation, scale);
					Math::DecomposeTransform(p2_mat, p2_coord, rotation, scale);
					Math::DecomposeTransform(p3_mat, p3_coord, rotation, scale);
					Math::DecomposeTransform(p4_mat, p4_coord, rotation, scale);
					Math::DecomposeTransform(p5_mat, p5_coord, rotation, scale);
					Math::DecomposeTransform(p6_mat, p6_coord, rotation, scale);
					Math::DecomposeTransform(p7_mat, p7_coord, rotation, scale);
					Math::DecomposeTransform(p8_mat, p8_coord, rotation, scale);

					Renderer2D::DrawLine(p3_coord, p1_coord, color);
					Renderer2D::DrawLine(p2_coord, p4_coord, color);
					Renderer2D::DrawLine(p1_coord, p2_coord, color);
					Renderer2D::DrawLine(p2_coord, p3_coord, color);
					Renderer2D::DrawLine(p3_coord, p4_coord, color);
					Renderer2D::DrawLine(p1_coord, p4_coord, color);
					Renderer2D::DrawLine(p1_coord, p5_coord, color);
					Renderer2D::DrawLine(p2_coord, p6_coord, color);
					Renderer2D::DrawLine(p3_coord, p7_coord, color);
					Renderer2D::DrawLine(p4_coord, p8_coord, color);
					Renderer2D::DrawLine(p5_coord, p6_coord, color);
					Renderer2D::DrawLine(p8_coord, p6_coord, color);
					Renderer2D::DrawLine(p8_coord, p7_coord, color);
					Renderer2D::DrawLine(p5_coord, p7_coord, color);
				} else {
					// -z direction
					glm::mat4 transform = tc.GetTransform();
					glm::mat4 p1_mat = glm::translate(glm::rotate(transform, cc.Camera.GetPerspectiveVerticalFOV(), {1, 1, 0}), { 0, 0, -100 });
					glm::mat4 p2_mat = glm::translate(glm::rotate(transform, cc.Camera.GetPerspectiveVerticalFOV(), { 1, -1, 0 }), { 0, 0, -100 });
					glm::mat4 p3_mat = glm::translate(glm::rotate(transform, cc.Camera.GetPerspectiveVerticalFOV(), { -1, 1, 0 }), { 0, 0, -100 });
					glm::mat4 p4_mat = glm::translate(glm::rotate(transform, cc.Camera.GetPerspectiveVerticalFOV(), { -1, -1, 0 }), { 0, 0, -100 });
					glm::vec3 p1_coord, p2_coord, p3_coord, p4_coord;
					glm::vec3 rotation, scale;
					Math::DecomposeTransform(p1_mat, p1_coord, rotation, scale);
					Math::DecomposeTransform(p2_mat, p2_coord, rotation, scale);
					Math::DecomposeTransform(p3_mat, p3_coord, rotation, scale);
					Math::DecomposeTransform(p4_mat, p4_coord, rotation, scale);

					Renderer2D::DrawLine(p1_coord, tc.Translation, color);
					Renderer2D::DrawLine(p2_coord, tc.Translation, color);
					Renderer2D::DrawLine(p3_coord, tc.Translation, color);
					Renderer2D::DrawLine(p4_coord, tc.Translation, color);
					Renderer2D::DrawLine(p1_coord, p2_coord, color);
					Renderer2D::DrawLine(p1_coord, p3_coord, color);
					Renderer2D::DrawLine(p3_coord, p4_coord, color);
					Renderer2D::DrawLine(p2_coord, p4_coord, color);
					Renderer2D::DrawLine(p1_coord, p4_coord, color);
					Renderer2D::DrawLine(p2_coord, p3_coord, color);
				}
			}
		}

		Renderer2D::EndScene();
	}

	void EditorLayer::UpdateEditor(Timestep ts) {
		// resizing
		if (!m_EditorParams->Resized) {
			glm::vec2 viewportSize = GetSpecificPanel<ViewportPanel>("Viewport")->GetViewportSize();
			m_EditorParams->ActiveScene->OnViewportResize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_EditorParams->Resized = true;
		}

		// close panels
		m_EditorParams->ClosedPanels.clear();
		for (auto& panel : m_Panels) {
			if (!panel.second->m_Enabled)
				m_EditorParams->ClosedPanels.push_back(panel.first);
		}

		// copy cut paste delete
		static bool copycutpasted = false;
		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		if (control && Input::IsKeyPressed(Key::C)) { // copy
			if (!copycutpasted) {
				switch (m_EditorParams->FocusedPanel) {
				case Panels::CONTENTBROWSER:
					m_EditorParams->Clipboard.Filepath = GetSpecificPanel<ContentBrowserPanel>("Content Browser")->GetSelectedFile();
					m_EditorParams->Clipboard.CutFile = false;
					break;
				case Panels::SCENEHIERARCHY:
					m_EditorParams->Clipboard.Entity = m_EditorParams->SelectedEntity;
					m_EditorParams->Clipboard.CutEntity = false;
					break;
				case Panels::VIEWPORT:
					m_EditorParams->Clipboard.Entity = m_EditorParams->SelectedEntity;
					m_EditorParams->Clipboard.CutEntity = false;
					break;
				}
				copycutpasted = true;
			}
		} else if (control && Input::IsKeyPressed(Key::X)) { // cut
			if (!copycutpasted) {
				switch (m_EditorParams->FocusedPanel) {
				case Panels::CONTENTBROWSER:
					m_EditorParams->Clipboard.Filepath = GetSpecificPanel<ContentBrowserPanel>("Content Browser")->GetSelectedFile();
					m_EditorParams->Clipboard.CutFile = true;
					break;
				case Panels::SCENEHIERARCHY:
					m_EditorParams->Clipboard.Entity = m_EditorParams->SelectedEntity;
					m_EditorParams->Clipboard.CutEntity = true;
					break;
				case Panels::VIEWPORT:
					m_EditorParams->Clipboard.Entity = m_EditorParams->SelectedEntity;
					m_EditorParams->Clipboard.CutEntity = true;
					break;
				}
				copycutpasted = true;
			}
		}
		else if (control && Input::IsKeyPressed(Key::V)) { // paste
			if (!copycutpasted) {
				switch (m_EditorParams->FocusedPanel) {
				case Panels::CONTENTBROWSER:
					if (!m_EditorParams->Clipboard.CutFile) {
						std::string copyfile = m_EditorParams->Clipboard.Filepath;
						if (copyfile == "") break;
						if (std::filesystem::is_directory(copyfile)) {
							std::string newFolder = GetSpecificPanel<ContentBrowserPanel>("Content Browser")->GetBrowserDirectory() + "\\"
								+ std::filesystem::path(copyfile).filename().string();
							int i = 0;
							while (std::filesystem::exists(newFolder)) {
								i++;
								newFolder = std::filesystem::path(copyfile).parent_path().string() + "/" +
									std::filesystem::path(copyfile).stem().string() + " (" + std::to_string(i) + ")" +
									std::filesystem::path(copyfile).extension().string();
							}
							FileUtils::CopyDirectory(copyfile, newFolder);
						}
						else {
							std::string newfile = GetSpecificPanel<ContentBrowserPanel>("Content Browser")->GetBrowserDirectory() + "\\"
								+ std::filesystem::path(copyfile).filename().string();
							int i = 0;
							while (std::filesystem::exists(newfile)) {
								i++;
								newfile = std::filesystem::path(copyfile).parent_path().string() + "/" +
									std::filesystem::path(copyfile).stem().string() + " (" + std::to_string(i) + ")" +
									std::filesystem::path(copyfile).extension().string();
							}
							std::ifstream infile(copyfile.c_str(), std::ios::binary);
							std::ofstream outfile(newfile.c_str(), std::ios::binary);
							if (infile.is_open() && outfile.is_open()) {
								outfile << infile.rdbuf();
								infile.close();
								outfile.close();
							}
							else FL_CORE_ERROR("Error pasting file");
						}
					} else {
						std::string copyfile = m_EditorParams->Clipboard.Filepath;
						std::string newfile = GetSpecificPanel<ContentBrowserPanel>("Content Browser")->GetBrowserDirectory() + "\\" +
							std::filesystem::path(copyfile).stem().string() +
							std::filesystem::path(copyfile).extension().string();
						if (std::rename(copyfile.c_str(), newfile.c_str()) != 0)
							FL_CORE_ERROR("Error moving file");
						else GetSpecificPanel<ContentBrowserPanel>("Content Browser")->SetSelectedFile("");
					}
					break;
				case Panels::SCENEHIERARCHY:
					if (!(bool)(m_EditorParams->SelectedEntity))
						m_EditorParams->ActiveScene->CopyEntity(m_EditorParams->Clipboard.Entity);
					else
						m_EditorParams->ActiveScene->CopyEntity(m_EditorParams->Clipboard.Entity, m_EditorParams->SelectedEntity);
					if (m_EditorParams->Clipboard.CutEntity) {
						if (m_EditorParams->SelectedEntity == m_EditorParams->Clipboard.Entity) {
							m_EditorParams->SelectedEntity = {};
						}
						GetSpecificPanel<SceneHierarchyPanel>("Scene Hierarchy")->DeleteEntity(m_EditorParams->Clipboard.Entity);
						m_EditorParams->Clipboard.Entity = {};
						m_EditorParams->Clipboard.CutEntity = false;
					}
					break;
				case Panels::VIEWPORT:
					if (!(bool)m_EditorParams->SelectedEntity)
						m_EditorParams->ActiveScene->CopyEntity(m_EditorParams->Clipboard.Entity);
					else
						m_EditorParams->ActiveScene->CopyEntity(m_EditorParams->Clipboard.Entity, m_EditorParams->SelectedEntity);
					if (m_EditorParams->Clipboard.CutEntity) {
						if (m_EditorParams->SelectedEntity == m_EditorParams->Clipboard.Entity) {
							m_EditorParams->SelectedEntity = {};
						}
						GetSpecificPanel<SceneHierarchyPanel>("Scene Hierarchy")->DeleteEntity(m_EditorParams->Clipboard.Entity);
						m_EditorParams->Clipboard.Entity = {};
						m_EditorParams->Clipboard.CutEntity = false;
					}
					break;
				}
				copycutpasted = true;
			}
		} else if (Input::IsKeyPressed(Key::Delete)) { // delete
			if (!copycutpasted) {
				std::filesystem::path path;
				switch (m_EditorParams->FocusedPanel) {
				case Panels::CONTENTBROWSER:
					path = std::filesystem::path(GetSpecificPanel<ContentBrowserPanel>("Content Browser")->GetSelectedFile());
					if (std::filesystem::is_directory(path)) {
						std::filesystem::remove_all(path.string().c_str());
					}
					else {
						if (std::remove(path.string().c_str()) != 0) FL_CORE_ERROR("Error deleting file");
					}
					GetSpecificPanel<ContentBrowserPanel>("Content Browser")->SetSelectedFile("");
					break;
				case Panels::VIEWPORT:
					GetSpecificPanel<SceneHierarchyPanel>("Scene Hierarchy")->DeleteEntity(m_EditorParams->SelectedEntity);
					m_EditorParams->SelectedEntity = {};
					break;
				case Panels::SCENEHIERARCHY:
					GetSpecificPanel<SceneHierarchyPanel>("Scene Hierarchy")->DeleteEntity(m_EditorParams->SelectedEntity);
					m_EditorParams->SelectedEntity = {};
					break;
				}
				copycutpasted = true;
			}
		} else if (copycutpasted) {
			copycutpasted = false;
		}

		// update params
		m_EditorParams->EditorCamera.OnUpdate(ts, m_EditorParams->FocusedPanel == Panels::VIEWPORT);
		if (m_EditorParams->SceneState == SceneState::EDIT)
			m_EditorParams->ActiveScene->OnUpdateEditor(ts, m_EditorParams->EditorCamera.GetViewProjection());
		else if (m_EditorParams->SceneState == SceneState::PLAY) {
			if (*(m_EditorParams->EditorCamera.GetCameraBound()))
				m_EditorParams->ActiveScene->OnUpdateRuntime(ts, m_EditorParams->EditorCamera.GetViewProjection());
			else
				m_EditorParams->ActiveScene->OnUpdateRuntime(ts);
		}
		m_EditorParams->Time += ts.GetSeconds();
		m_EditorParams->HoveredPanel = Panels::NONE;
		m_EditorParams->FocusedPanel = Panels::NONE;
	}

	void EditorLayer::AutoSaveEditor(Timestep ts) {
		static float time = 0.0f;
		time += ts;
		if (time > 300) { // save settings every 5 minutes
			time = 0.0f;
			FileUtils::SaveEditor(m_EditorParams);
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
				m_EditorParams->Time = 0;
			} else if (control && Input::IsKeyPressed(Key::S)) {
				FileUtils::SaveScene(m_EditorParams);
				m_EditorParams->Time = 0;
			} else if (control && Input::IsKeyPressed(Key::N)) {
				PromptSave(SavePromptType::NEW);
			} else if (control && Input::IsKeyPressed(Key::O)) {
				PromptSave(SavePromptType::OPEN);
			} else if (Input::IsKeyPressed(Key::Backslash)) {
				DevEvent();
			} else {
				m_OverrideEventReady = true;
			}
		} else {
			m_OverrideEventReady = true;
		}
	}

	void EditorLayer::DevEvent() {
		FL_CORE_INFO("DEV EVENT FIRED");
		ScriptEngine::ReloadAssembly();
		//AudioCommand::Init();
	}
}