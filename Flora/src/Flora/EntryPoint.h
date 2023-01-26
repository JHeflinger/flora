#pragma once

#ifdef FL_PLATFORM_WINDOWS

extern Flora::Application* Flora::CreateApplication();

int main(int argc, char** argv) {
	printf("WELCOME TO FLORA ENGINE\n");
	Flora::Log::Init();
	auto app = Flora::CreateApplication();
	app->Run();
	delete app;
}

#endif