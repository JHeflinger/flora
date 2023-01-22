#include <Flora.h>

class Sandbox : public Flora::Application {
public:
	Sandbox() {

	}
	~Sandbox() {

	}
};

Flora::Application* Flora::CreateApplication() {
	return new Sandbox();
}