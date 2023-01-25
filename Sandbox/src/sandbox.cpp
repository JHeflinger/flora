#include <Flora.h>

class ExampleLayer : public Flora::Layer {
public:
	ExampleLayer() : Layer("Example") {
	}

	void OnUpdate() override {
		FL_INFO("ExmapleLayer::Update");
	}

	void OnEvent(Flora::Event& event) override {
		FL_TRACE("{0}", event);
	}
};

class Sandbox : public Flora::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
};

Flora::Application* Flora::CreateApplication() {
	return new Sandbox();
}