#pragma once
#include "Flora/Core/Application.h"

extern Flora::Application* Flora::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv) {
	Flora::Log::Init();
	auto app = Flora::CreateApplication({ argc, argv });
	app->Run();
	delete app;
	return 0;
}