#pragma once

namespace Flora {
	struct ProjectConfig {
		std::string Name = "Untitled";
		std::filesystem::path StartScene;
		std::filesystem::path AssetDirectory;
	};

	class Project {
	public:
		static const std::filesystem::path& GetProjectDirectory() {
			FL_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->m_ProjectDirectory;
		}

		static std::filesystem::path GetAssetDirectory() {
			FL_CORE_ASSERT(s_ActiveProject);
			return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
		}

		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path) {
			FL_CORE_ASSERT(s_ActiveProject);
			return GetAssetDirectory() / path;
		}
	public:
		ProjectConfig& GetConfig() { return m_Config; }
		static Ref<Project> GetActive() { return s_ActiveProject; }
		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static bool SaveActive(const std::filesystem::path& path);
		static bool GenerateProjectDirectory(const std::string name, const std::filesystem::path& path);
		static bool RegenerateScriptingProject();
	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;
		inline static Ref<Project> s_ActiveProject;
	};

}