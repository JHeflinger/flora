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
	};
}