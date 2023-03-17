#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D() 
	: Layer("Example"), 
	m_CameraController(1280.0f / 720.0f, true) {

}

void Sandbox2D::OnAttatch() {
	m_CheckerboardTexture = Flora::Texture2D::Create("assets/textures/test.png");

	Flora::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Flora::Framebuffer::Create(fbSpec);
}

void Sandbox2D::OnDetatch() {

}

void Sandbox2D::OnUpdate(Flora::Timestep ts) {
	FL_PROFILE_FUNCTION();

	{
		FL_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	{
		FL_PROFILE_SCOPE("Renderer Prep");
		m_Framebuffer->Bind();
		Flora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Flora::RenderCommand::Clear();
		Flora::Renderer2D::ResetStats();
	}
	
	{
		FL_PROFILE_SCOPE("Rendering!");
		Flora::Renderer2D::BeginScene(m_CameraController.GetCamera());
		static float rotation = 0.0f;
		rotation += ts * 20.0f;
		Flora::Renderer2D::DrawQuad({ 0, 0 }, { 1, 1 }, m_CheckerboardTexture, {0.8f, 0.3f, 0.2f, 1.0f}, glm::radians(rotation));
		Flora::Renderer2D::EndScene();
		m_Framebuffer->Unbind();
	}
}

void Sandbox2D::OnImGuiRender() {
	FL_PROFILE_FUNCTION();
	
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
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("EXIT")) Flora::Application::Get().Close();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	//put dockable imgui panels here
	ImGui::Begin("Renderer Stats");
	auto stats = Flora::Renderer2D::GetStats();
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)textureID, ImVec2{ 1280.0f, 720.0f });

	ImGui::End();

	ImGui::End();
}

void Sandbox2D::OnEvent(Flora::Event& e) {
	m_CameraController.OnEvent(e);
}