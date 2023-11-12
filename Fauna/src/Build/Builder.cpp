#include "flpch.h"
#include "Builder.h"
#include "Flora/Project/Project.h"

namespace Flora {
	static BuildConfig s_Config;
	static int s_State = 0;
	static std::vector<std::filesystem::path> s_ScenePaths;
	static std::string s_SceneHeader = "";

	std::vector<std::string> Builder::VerifyConfig(BuildConfig config) {
		std::vector<std::string> errors;
		if (config.Name.size() == 0) errors.push_back("ERROR >> Name is a required field - please fill it out");

		if (config.IconPath != "") {
			if (std::filesystem::exists(config.IconPath)) {
				std::string extension = std::filesystem::path(config.IconPath).extension().string();
				for (char& c : extension) c = std::tolower(c);
				if (extension != ".png") errors.push_back("ERROR >> Please use a PNG file for the icon");
			} else errors.push_back("ERROR >> Icon path does not point to an existing file");
		}

		if (config.OutputDirectory.size() != 0) {
			if (!std::filesystem::is_directory(config.OutputDirectory))
				errors.push_back("ERROR >> Output directory does not exist");
		} else errors.push_back("ERROR >> Output directory is a required field - please fill it out");

		if (!config.WinBuild && !config.LinBuild && !config.MacBuild)
			errors.push_back("ERROR >> Build platform is a required field - please select a platform");

		s_Config = config;
		s_State = 0;

		return errors;
	}

	std::string Builder::StepBuild() {
		std::string output = "INFO >> Finished building!";
		switch (s_State) {
		case 0:
			s_State++;
			ResetSceneHeader();
			output = "INFO >> Compressing scenes...";
			GetScenes();
			break;
		case 1:
			if (s_ScenePaths.size() == 0) { s_State++; return StepBuild(); }
			else {
				output = "Compressing " + s_ScenePaths[0].filename().string() + "...";
				if (!CompressScene(s_ScenePaths[0])) output = "ERROR >> Unable to compress " + s_ScenePaths[0].filename().string();
				s_ScenePaths.erase(s_ScenePaths.begin());
			}
			break;
		case 2:
			s_State++;
			output = "INFO >> Writing compressed scenes to header...";
			break;
		case 3:
			s_State++;
			if (!WriteSceneHeader()) output = "ERROR >> Unable to write compressed scenes to header";
			break;
		}
		return output;
	}

	bool Builder::WriteSceneHeader() {
		FinishSceneHeader();
		std::ofstream outputFile(s_Config.OutputDirectory + "\\SceneManager.cpp");
		if (outputFile.is_open()) {
			outputFile << s_SceneHeader;
			outputFile.close();
			return true;
		}
		return false;
	}

	void Builder::GetScenes() {
		s_ScenePaths.clear();
		for (const auto& entry : std::filesystem::recursive_directory_iterator(Project::GetAssetDirectory()))
			if (entry.is_regular_file())
				if(entry.path().extension().string() == ".flora")
					s_ScenePaths.push_back(entry.path());
	}

	bool Builder::CompressScene(std::filesystem::path filepath) {
		std::pair<bool, std::string> scenecontent = FileToString(filepath);
		if (!scenecontent.first) return false;
		std::string scenepath = filepath.string();
		std::replace(scenepath.begin(), scenepath.end(), '\\', '/');
		std::replace(scenecontent.second.begin(), scenecontent.second.end(), '\\', '/');
		s_SceneHeader += "\t\tm_SceneMap[\"" + scenepath + "\"] = R\"(" + scenecontent.second + ")\";\n";
		return true;
	}

	std::pair<bool, std::string> Builder::FileToString(std::filesystem::path filepath) {
		std::ifstream inputFile(filepath);
		if (inputFile.is_open()) {
			std::string content((std::istreambuf_iterator<char>(inputFile)), (std::istreambuf_iterator<char>()));
			return { true, content };
		}
		else return { false, "" };
	}

	void Builder::ResetSceneHeader() {
		s_SceneHeader = "#include \"flpch.h\"\n#include \"SceneManager.h\"\n\nnamespace Flora {\n\tSceneManager::SceneManager() {\n";
	}

	void Builder::FinishSceneHeader() {
		s_SceneHeader += "\t}\n\n\tstd::string SceneManager::GetScene(std::string scene) {"
			"\n\t\tstd::replace(scene.begin(), scene.end(), \'\\\\\', \'/\');"
			"\n\t\tif (m_SceneMap.find(scene) == m_SceneMap.end()) return \"\";"
			"\n\t\treturn m_SceneMap[scene];\n\t}\n}";
	}
}