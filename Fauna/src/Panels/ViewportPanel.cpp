#include "flpch.h"
#include "ViewportPanel.h"
#include "Flora/Renderer/Renderer2D.h"
#include "Flora/Renderer/RenderCommand.h"
#include "Flora/Core/Application.h"
#include "Flora/Core/Input.h"
#include "Flora/Math/Math.h"
#include "../Utils/FileUtils.h"
#include "../Utils/ComponentUtils.h"
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "ImGuizmo.h"

namespace Flora {
	//temp, remove when projects are implemented
	extern const std::filesystem::path g_AssetPath;

	void ViewportPanel::Initialize() {
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);
	}

	void ViewportPanel::PreUpdate() {
		// renderer setup
		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// clear entity ID attachment to -1
		m_Framebuffer->ClearAttachment(1, -1);
	}

	void ViewportPanel::OnUpdate() {
		if (ViewportResized()) {
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorContext->EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorContext->ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}
	}

	bool ViewportPanel::ViewportResized() {
		FramebufferSpecification spec = m_Framebuffer->GetSpecification();
		return m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y);
	}

	void ViewportPanel::PostUpdate() {
		// set hovered entity based on mouse data
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;
		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_EditorContext->ActiveScene.get());
		}

		// renderer "unsetup", move later
		m_Framebuffer->Unbind();
	}

	void ViewportPanel::OnImGuiRender() {
		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport", &m_Enabled);
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x , viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x , viewportMaxRegion.y + viewportOffset.y };
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		m_EditorContext->ActiveScene->SetViewportHovered(m_ViewportHovered); // temporary solution
		m_EditorContext->ActiveScene->SetViewportFocused(m_ViewportFocused);
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// Drag scenes into viewport capability
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path scenePath = std::filesystem::path(g_AssetPath) / path;
				if (scenePath.extension().string() != ".flora")
					FL_CORE_ERROR("Not a valid scene file! Please upload a file ending with the .flora extension"); //convert later to popup maybe
				else {
					RequestOpenScene(scenePath.string());
				}
			}
			ImGui::EndDragDropTarget();
		}

		// Viewport Gizmos
		Entity selectedEntity = m_EditorContext->SelectedEntity;
		if (selectedEntity && m_EditorContext->GizmoType != -1) {
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Editor Camera
			const glm::mat4& cameraProjection = m_EditorContext->EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorContext->EditorCamera.GetViewMatrix();

			// Runtime Camera
			/*auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			const glm::mat4& cameraProjection = camera.GetProjection();
			glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());*/

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();
			glm::mat4 parentTransform = ComponentUtils::GetParentTransform(selectedEntity);
			transform = parentTransform * transform;

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;
			if (m_EditorContext->GizmoType == ImGuizmo::OPERATION::ROTATE) snapValue = 45.0f;
			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_EditorContext->GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing()) {
				glm::vec3 translation, rotation, scale;
				glm::mat4 newTransform = selectedEntity.HasComponent<ParentComponent>() ? glm::inverse(parentTransform) * transform : transform;
				Math::DecomposeTransform(newTransform, translation, rotation, scale);
				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = glm::vec3(newTransform[3]);
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}

		// End Viewport
		ImGui::End();
		ImGui::PopStyleVar();
	}
}