#pragma once
#include <filesystem>

namespace Flora {
	class FileUtils {
	public:
		static void OpenScene();
		static void SaveSceneAs();
		static void SaveScene();
		static void OpenScene(const std::filesystem::path& path);
		static void NewScene();
	};
}