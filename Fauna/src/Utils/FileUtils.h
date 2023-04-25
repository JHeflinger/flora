#pragma once
#include <filesystem>
#include "Flora/Utils/Structures.h"

namespace Flora {
	class FileUtils {
	public:
		static void OpenScene(Ref<EditorParams> context);
		static void SaveSceneAs(Ref<EditorParams> context);
		static void SaveScene(Ref<EditorParams> context);
		static void OpenScene(Ref<EditorParams> context, const std::filesystem::path& path);
		static void NewScene(Ref<EditorParams> context);
	public:
		static void CopyDirectory(const std::filesystem::path& source, const std::filesystem::path& destination);
	};
}