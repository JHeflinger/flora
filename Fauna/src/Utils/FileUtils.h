#pragma once
#include <filesystem>
#include "Structures.h"

namespace Flora {
	class FileUtils {
	public:
		static void OpenScene(Ref<EditorParams> context);
		static void SaveSceneAs(Ref<EditorParams> context);
		static void SaveScene(Ref<EditorParams> context);
		static void ExportEntity(Entity entity);
		static Entity* ImportEntity(Ref<EditorParams> context);
		static Entity* ImportEntity(Ref<EditorParams> context, const std::filesystem::path& path);
		static void SaveTempScene(Ref<EditorParams> context);
		static void OpenTempScene(Ref<EditorParams> context);
		static void OpenScene(Ref<EditorParams> context, const std::filesystem::path& path);
		static void NewScene(Ref<EditorParams> context);
		static int ShellOpen(const std::filesystem::path path);
	public:
		static void SaveEditor(Ref<EditorParams> context);
		static void LoadEditor(Ref<EditorParams> context);
	public:
		static void CopyDirectory(const std::filesystem::path& source, const std::filesystem::path& destination);
	public:
		static void CreateScript(const std::filesystem::path& directory);
		static void CreateScene(const std::filesystem::path& directory, std::string filename = "Untitled Scene");
	};
}