#pragma once
#include "Flora/Project/Label.h"

namespace Flora {
	struct ProjectConfig {
		std::string Name = "Untitled";
		std::filesystem::path StartScene;
		std::filesystem::path AssetDirectory;
	};

	class Project {
	public:
		static const std::filesystem::path& GetProjectDirectory() {
			if (!s_ActiveProject) return "";
			return s_ActiveProject->m_ProjectDirectory;
		}

		static std::filesystem::path GetAssetDirectory() {
			if (!s_ActiveProject) return "";
			return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
		}

		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path) {
			if (!s_ActiveProject) return "";
			return GetAssetDirectory() / path;
		}
	public:
		ProjectConfig& GetConfig() { return m_Config; }
		Label* GetLabel(std::string label);
		bool AddLabel(std::string label);
		bool RemoveLabel(std::string label);
		bool LabelExists(std::string label);
		static Label* GetActiveLabel(std::string label) { return s_ActiveProject->GetActiveLabel(label); }
		static bool AddActiveLabel(std::string label) { return s_ActiveProject->AddLabel(label); }
		static bool RemoveActiveLabel(std::string label) { return s_ActiveProject->RemoveLabel(label); }
		static bool ActiveLabelExists(std::string label) { return s_ActiveProject->LabelExists(label); }
		std::unordered_map<std::string, Label*> GetLabels() { return m_Labels; }
		static std::unordered_map<std::string, Label*> GetActiveLabels() { return s_ActiveProject->GetLabels(); }
		static Ref<Project> GetActive() { return s_ActiveProject; }
		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static bool SaveActive(const std::filesystem::path& path);
		#ifdef WIN_BUILD_ONLY
		static bool GenerateProjectDirectory(const std::string name, const std::filesystem::path& path);
		static bool RegenerateScriptingProject();
		#endif
	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;
		inline static Ref<Project> s_ActiveProject;
		std::unordered_map<std::string, Label*> m_Labels;
	};

}