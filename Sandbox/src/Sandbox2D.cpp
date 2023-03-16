#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

static const uint32_t s_MapWidth = 24;
static const uint32_t s_MapHeight = 16;
static const char* s_MapTiles = "DDDDDDDDDDDDDDDDDDDDDDDD"
                                "DDDDDDDDDDDDDDDDDDDDDDDD"
                                "DDDDDDDDDDDDDDDDDDDDDDDD"
                                "DDDDDDDDDDDDDDDDDDDDDDDD"
                                "DDDDDDDDDDDDDDDDDDDDDDDD"
                                "DDDDDDDDDDDDDDDDDDDDDDDD"
                                "DDDDDRRRRRRRRRRDDDDDDDDD"
                                "DDDDDDDDDDDDDDRDDDDDDDDD"
                                "DDDDDDDDDDDDDDRDDDDDDDDD"
                                "DDDDDDDDDDDRRRRDDDDDDDDD"
                                "DDDDDDDDDDDRDDDDDDDDDDDD"
                                "DDDDDDDDDDDRDDDDDDDDDDDD"
                                "DDDDDDDDDDDRDDDDDDDDDDDD"
                                "DDDDDDDDDDDRRRRDDDDDDDDD"
                                "DDDDDDDDDDDDDDDDDDDDDDDD"
                                "DDDDDDDDDDDDDDDDDDDDDDDD";

Sandbox2D::Sandbox2D() 
	: Layer("Example"), 
	m_CameraController(1280.0f / 720.0f, true) {

}

void Sandbox2D::OnAttatch() {
	m_CheckerboardTexture = Flora::Texture2D::Create("assets/textures/test.png");
	m_SpriteSheet = Flora::Texture2D::Create("assets/game/textures/tilemap.png");
	m_WizardTexture = Flora::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 3 }, { 17, 17 });

	s_TextureMap['D'] = Flora::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 6 }, { 17, 17 });
	s_TextureMap['R'] = Flora::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 10, 3 }, { 17, 17 });

	m_Particle.ColorBegin = { 255 / 255.0f, 255 / 255.0f, 0 / 143.0f, 1.0f };
	m_Particle.ColorEnd = { 255 / 255.0f, 0 / 255.0f, 0 / 255.0f, 0.0f };
	m_Particle.ColorVariation = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 0.75f;
	m_Particle.LifeTimeVariation = 0.0f;
	m_Particle.Velocity = { 0.0f, 6.0f };
	m_Particle.VelocityVariation = { 6.0f, 3.0f };
	m_Particle.Position = { 0.0f, 0.0f };
	m_Particle.PositionVariation = { 0.5f, 0.5f };

	m_CameraController.SetZoomLevel(5.0f);
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
		Flora::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Flora::RenderCommand::Clear();
		Flora::Renderer2D::ResetStats();
	}
	/*
	{
		FL_PROFILE_SCOPE("Renderer Draw");
		Flora::Renderer2D::BeginScene(m_CameraController.GetCamera());

		static float rotation = 0.0f;
		static float rotation2 = 0.0f;
		static float rotation3 = 0.0f;
		rotation += ts * 20.0f;
		rotation2 += ts * 190.0f;
		rotation3 -= ts * 360.0f;

		static float red = 1.0f;
		static float green = 0.0f;
		static float blue = 0.0f;
		static float colorspeed = 1.5f;

		if (red >= 1.0f && blue <= 0.0f && green < 1.0f) {
			green += ts * colorspeed;
		}
		else if (red >= 0.0f && green >= 1.0f && red > 0.0f) {
			red -= ts * colorspeed;
		}
		else if (red <= 0.0f && green >= 1.0f && blue < 1.0f) {
			blue += ts * colorspeed;
		}
		else if (blue >= 1.0f && red <= 0.0f && green > 0.0f) {
			green -= ts * colorspeed;
		}
		else if (green <= 0.0f && blue >= 1.0f && red < 1.0f) {
			red += ts * colorspeed;
		}
		else if (red >= 1.0f && green <= 0.0f && blue > 0.0f) {
			blue -= ts * colorspeed;
		}

		m_SquareColor.x = red;
		m_SquareColor.y = green;
		m_SquareColor.z = blue;

		//commented out until able to debug
		
		//15 quads
		Flora::Renderer2D::DrawQuad({0.0f, 0.0f, -0.9f}, {0.8f, 0.8f}, {m_SquareColor.y, m_SquareColor.z, m_SquareColor.x, m_SquareColor.w}, nullptr, rotation, 1.0f);
		Flora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.8f }, { 0.5f, 0.5f }, { m_SquareColor.x, m_SquareColor.y, m_SquareColor.z, m_SquareColor.w }, m_CheckerboardTexture, glm::radians(rotation2), 5.0f);
		Flora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.7f }, { 0.6f, 0.6f }, { 1.0f - m_SquareColor.x, 1.0f - m_SquareColor.y, 1.0f - m_SquareColor.z, m_SquareColor.w }, m_DumbTexture, rotation3);

		Flora::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.9f }, { 0.4f, 0.4f }, { m_SquareColor.x, m_SquareColor.y, m_SquareColor.z, m_SquareColor.w }, nullptr, rotation * -3.8, 1.0f);
		Flora::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.8f }, { 0.25f, 0.25f }, { m_SquareColor.y, m_SquareColor.z, m_SquareColor.z, m_SquareColor.w }, m_CheckerboardTexture, glm::radians(rotation2 * -3.8), 5.0f);
		Flora::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.7f }, { 0.3f, 0.3f }, { 1.0f - m_SquareColor.y, 1.0f - m_SquareColor.z, 1.0f - m_SquareColor.x, m_SquareColor.w }, m_DumbTexture, rotation3 * -3.8);

		Flora::Renderer2D::DrawQuad({ 1.0f, 0.0f, -0.9f }, { 0.4f, 0.4f }, { m_SquareColor.x, m_SquareColor.z, m_SquareColor.y, m_SquareColor.w }, nullptr, rotation * 0.5, 1.0f);
		Flora::Renderer2D::DrawQuad({ 1.0f, 0.0f, -0.8f }, { 0.25f, 0.25f }, { m_SquareColor.z, m_SquareColor.y, m_SquareColor.x, m_SquareColor.w }, m_CheckerboardTexture, glm::radians(rotation2 * 0.5), 5.0f);
		Flora::Renderer2D::DrawQuad({ 1.0f, 0.0f, -0.7f }, { 0.3f, 0.3f }, { 1.0f - m_SquareColor.z, 1.0f - m_SquareColor.y, 1.0f - m_SquareColor.x, m_SquareColor.w }, m_DumbTexture, rotation3 * 0.5);

		Flora::Renderer2D::DrawQuad({ 0.0f, -1.0f, -0.9f }, { 0.4f, 0.4f }, { m_SquareColor.z, m_SquareColor.y, m_SquareColor.x, m_SquareColor.w }, nullptr, rotation * 8.5, 1.0f);
		Flora::Renderer2D::DrawQuad({ 0.0f, -1.0f, -0.8f }, { 0.25f, 0.25f }, { m_SquareColor.z, m_SquareColor.x, m_SquareColor.y, m_SquareColor.w }, m_CheckerboardTexture, glm::radians(rotation2 * 7.5), 5.0f);
		Flora::Renderer2D::DrawQuad({ 0.0f, -1.0f, -0.7f }, { 0.3f, 0.3f }, { 1.0f - m_SquareColor.x, 1.0f - m_SquareColor.z, 1.0f - m_SquareColor.y, m_SquareColor.w }, m_DumbTexture, rotation3 * 0.5);

		Flora::Renderer2D::DrawQuad({ 0.0f, 1.0f, -0.9f }, { 0.4f, 0.4f }, { m_SquareColor.x, m_SquareColor.z, m_SquareColor.x, m_SquareColor.w }, nullptr, rotation * 9.5, 1.0f);
		Flora::Renderer2D::DrawQuad({ 0.0f, 1.0f, -0.8f }, { 0.25f, 0.25f }, { m_SquareColor.y, m_SquareColor.y, m_SquareColor.z, m_SquareColor.w }, m_CheckerboardTexture, glm::radians(rotation2 * 2.5), 5.0f);
		Flora::Renderer2D::DrawQuad({ 0.0f, 1.0f, -0.7f }, { 0.3f, 0.3f }, { 1.0f - m_SquareColor.z, 1.0f - m_SquareColor.x, 1.0f - m_SquareColor.y, m_SquareColor.w }, m_DumbTexture, rotation3 * 3.5);

		Flora::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.999f }, { 40.0f, 40.0f }, {1.0f, 1.0f, 1.0f, 1.0f}, m_CheckerboardTexture, 0.0f, 20.0f);
		float bounds = 10.0f;
		float stepsize = 0.5f;
		float spacing = 0.05f;
		for (float y = -bounds; y < bounds; y += stepsize) {
			for (float x = -bounds; x < bounds; x += stepsize) {
				glm::vec4 color = { (x + bounds) / bounds * 2, 0.4f, (y + bounds) / bounds * 2, 0.5f };
				Flora::Renderer2D::DrawQuad({ x, y, -0.99f }, { stepsize - spacing, stepsize - spacing }, color);
			}
		}
		Flora::Renderer2D::EndScene();
	}
	*/
	{
		FL_PROFILE_SCOPE("TILEMAP TEST");
		Flora::Renderer2D::BeginScene(m_CameraController.GetCamera());
		//Flora::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_WizardTexture);

		for (uint32_t y = 0; y < s_MapHeight; y++) {
			for (uint32_t x = 0; x < s_MapWidth; x++) {
				char tileType = s_MapTiles[x + y * s_MapWidth];
				Flora::Ref<Flora::SubTexture2D> texture;
				if (s_TextureMap.find(tileType) != s_TextureMap.end())
					texture = s_TextureMap[tileType];
				else
					texture = m_WizardTexture;
				Flora::Renderer2D::DrawQuad({ x - s_MapWidth / 2.0f, y - s_MapHeight / 2.0f }, { 1.0f, 1.0f }, texture);
			}
		}

		Flora::Renderer2D::EndScene();
	}
	
	{
		FL_PROFILE_SCOPE("PARTICLE RENDER TEST");

		Flora::Renderer2D::BeginScene(m_CameraController.GetCamera());
		if (Flora::Input::IsMouseButtonPressed(FL_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Flora::Input::GetMousePosition();
			auto width = Flora::Application::Get().GetWindow().GetWidth();
			auto height = Flora::Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 20; i++)
				m_ParticleSystem.Emit(m_Particle);
		}

		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender(m_CameraController.GetCamera());
		Flora::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender() {
	FL_PROFILE_FUNCTION();
	/*


	//ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::Begin("Particle Settings");
	ImGui::SliderFloat("X Variation", &m_Particle.PositionVariation.x, 0, 10, "%.3f", 0);
	ImGui::SliderFloat("Y Variation", &m_Particle.PositionVariation.y, 0, 10, "%.3f", 0);
	ImGui::SliderFloat("X Velocity", &m_Particle.Velocity.x, 0, 100, "%.3f", 0);
	ImGui::SliderFloat("Y Velocity", &m_Particle.Velocity.y, 0, 100, "%.3f", 0);
	ImGui::SliderFloat("X Velocity Variation", &m_Particle.VelocityVariation.x, 0, 100, "%.3f", 0);
	ImGui::SliderFloat("Y Velocity Variation", &m_Particle.VelocityVariation.y, 0, 100, "%.3f", 0);
	ImGui::SliderFloat("RED Variation", &m_Particle.ColorVariation.x, 0, 10, "%.3f", 0);
	ImGui::SliderFloat("GREEN Variation", &m_Particle.ColorVariation.y, 0, 1, "%.3f", 0);
	ImGui::SliderFloat("BLUE Variation", &m_Particle.ColorVariation.z, 0, 1, "%.3f", 0);
	ImGui::SliderFloat("ALPHA Variation", &m_Particle.ColorVariation.w, 0, 1, "%.3f", 0);
	ImGui::SliderFloat("Size Variation", &m_Particle.SizeVariation, 0, 20, "%.3f", 0);
	ImGui::SliderFloat("Start Size", &m_Particle.SizeBegin, 0, 20, "%.3f", 0);
	ImGui::SliderFloat("End Size", &m_Particle.SizeEnd, 0, 20, "%.3f", 0);
	ImGui::SliderFloat("Lifetime", &m_Particle.LifeTime, 0, 2, "%.3f", 0);
	ImGui::SliderFloat("Lifetime Variation", &m_Particle.LifeTimeVariation, 0, 2, "%.3f", 0);
	ImGui::End();*/
	static bool dockspace_open = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspace_open, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
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

	uint32_t textureID = m_CheckerboardTexture->GetRendererID();
	ImGui::Image((void*)textureID, ImVec2{ 64.0f, 64.0f });

	ImGui::End();

	ImGui::End();
}

void Sandbox2D::OnEvent(Flora::Event& e) {
	m_CameraController.OnEvent(e);
}