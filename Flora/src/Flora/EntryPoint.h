#pragma once

#ifdef FL_PLATFORM_WINDOWS

extern Flora::Application* Flora::CreateApplication();

int main(int argc, char** argv) {
	printf("WELCOME TO FLORA ENGINE\n");
	Flora::Log::Init();
	FL_CORE_WARN("my warning");
	int a = 10;
	FL_INFO("my info={0}", a);
	auto app = Flora::CreateApplication();
	app->Run();
	delete app;
}

#endif