#pragma once

#ifdef FL_PLATFORM_WINDOWS

extern Flora::Application* Flora::CreateApplication();

int main(int argc, char** argv) {
	Flora::Log::Init();

	FL_PROFILE_BEGIN_SESSION("Startup", "debug_log/FloraProfile_Startup.json");
	auto app = Flora::CreateApplication();
	FL_PROFILE_END_SESSION();

	FL_PROFILE_BEGIN_SESSION("Runtime", "debug_log/FloraProfile_Runtime.json");
	app->Run();
	FL_PROFILE_END_SESSION();

	FL_PROFILE_BEGIN_SESSION("Shutdown", "debug_log/FloraProfile_Shutdown.json");
	delete app;
	FL_PROFILE_END_SESSION();
}

#endif