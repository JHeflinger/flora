#pragma once

namespace Flora {
	struct BuildConfig {
		std::string Name = "";
		std::string IconPath = "";
		std::string OutputDirectory = "";
		bool WinBuild = false;
		bool LinBuild = false;
		bool MacBuild = false;
		bool Fullscreen = false;
		int AppWidth = 0;
		int AppHeight = 0;
	};

	class Builder {
	public:
		static std::vector<std::string> VerifyConfig(BuildConfig config);
		static std::string StepBuild();
	private:
		static void FinishSceneHeader();
		static void ResetSceneHeader();
		static bool WriteSceneHeader();
		static void GetScenes();
		static bool CompressScene(std::filesystem::path filepath);
		static std::pair<bool, std::string> FileToString(std::filesystem::path filepath);
	};
}