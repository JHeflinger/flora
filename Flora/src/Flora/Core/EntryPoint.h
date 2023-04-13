#pragma once
#include "Flora/Core/Base.h"
#include "Flora/Core/Application.h"

#ifdef FL_PLATFORM_WINDOWS

extern Flora::Application* Flora::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv) {
	Flora::Log::Init();

	// warnings
	{
		FL_WARN("Session does not indicate save status");
		FL_WARN("Gizmos are not saved in between sessions");
		FL_WARN("Console messages are only reflected in the terminal, not fauna");
		FL_CORE_WARN("Files are not checked for validity when uploading to textures, scenes, and scripts");
		FL_CORE_WARN("There is no assetmanager to minimize texture allocation");
		FL_WARN("Undo and redo is not currently implemented");
		FL_CORE_WARN("Native script path is unstable");
	}

	FL_PROFILE_BEGIN_SESSION("Startup", "debug_log/FloraProfile_Startup.json");
	auto app = Flora::CreateApplication({ argc, argv });
	FL_PROFILE_END_SESSION();

	FL_PROFILE_BEGIN_SESSION("Runtime", "debug_log/FloraProfile_Runtime.json");
	app->Run();
	FL_PROFILE_END_SESSION();

	FL_PROFILE_BEGIN_SESSION("Shutdown", "debug_log/FloraProfile_Shutdown.json");
	delete app;
	FL_PROFILE_END_SESSION();
}

#endif