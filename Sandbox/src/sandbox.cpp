#include <Flora.h>
#include "imgui/imgui.h"

class ExampleLayer : public Flora::Layer {
public:
	ExampleLayer() : Layer("Example") {
	}

	void OnUpdate() override {
		if (Flora::Input::IsKeyPressed(FL_KEY_TAB))
			FL_INFO("Tab key is being pressed");
	}

	void OnEvent(Flora::Event& event) override {
		FL_TRACE("{0}", event);
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("test");
		ImGui::Text("Hello world");
		ImGui::End();
	}
};

class Sandbox : public Flora::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		//PushOverlay(new Flora::ImGuiLayer());
	}
	~Sandbox() {

	}
};

Flora::Application* Flora::CreateApplication() {
	return new Sandbox();
}