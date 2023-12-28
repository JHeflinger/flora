#include "flpch.h"
#include "Project.h"
#include "Flora/Project/ProjectSerializer.h"

namespace Flora { 
	Ref<Project> Project::New() {
		s_ActiveProject = CreateRef<Project>();
		return s_ActiveProject;
	}

	Ref<Project> Project::Load(const std::filesystem::path& path) {
		Ref<Project> project = CreateRef<Project>();
		ProjectSerializer serializer(project);
		if (serializer.Deserialize(path)) {
			project->m_ProjectDirectory = path.parent_path();
			s_ActiveProject = project;
			return s_ActiveProject;
		}

		return nullptr;
	}

	bool Project::SaveActive(const std::filesystem::path& path) {
		ProjectSerializer serializer(s_ActiveProject);
		if (serializer.Serialize(path)) {
			s_ActiveProject->m_ProjectDirectory = path.parent_path();
			return true;
		}

		return false;
	}

#ifdef WIN_BUILD_ONLY
	bool Project::GenerateProjectDirectory(const std::string name, const std::filesystem::path& path) {
		std::filesystem::path assetFolder = path / "Assets";
		std::filesystem::path textureFolder = assetFolder / "Textures";
		std::filesystem::path scriptFolder = assetFolder / "Scripts";
		std::filesystem::path sceneFolder = assetFolder / "Scenes";
		std::filesystem::path audioFolder = assetFolder / "Audio";
		std::filesystem::path entityFolder = assetFolder / "Entities";
		std::filesystem::create_directory(assetFolder);
		std::filesystem::create_directory(textureFolder);
		std::filesystem::create_directory(scriptFolder);
		std::filesystem::create_directory(sceneFolder);
		std::filesystem::create_directory(audioFolder);
		std::filesystem::create_directory(entityFolder);

		// create intermediates dir
		std::filesystem::path intermediate_path = std::filesystem::path("scripting/Project Intermediates") / name;
		std::filesystem::create_directory(intermediate_path);

		// create script folder path
		size_t pos = 0;
		std::string scriptsPath = scriptFolder.string();
		while ((pos = scriptsPath.find("\\", pos)) != std::string::npos) {
			scriptsPath.replace(pos, 1, "/");
			pos += 1;
		}

		// create custom premake
		std::ofstream premake("scripting/premake5.lua");
		if (premake.is_open()) {
			std::string filecontent =
				"local FloraRootDir = \'../../\'\n"
				"include(FloraRootDir .. \"/vendor/premake/premake_customization/solution_items.lua\")\n"
				"\n"
				"workspace \"" + name + "\"\n"
				"architecture \"x86_64\"\n"
				"startproject \"" + name + "\"\n"
				"\n"
				"configurations\n"
				"{\n"
				"\"Debug\",\n"
				"\"Release\",\n"
				"\"Dist\"\n"
				"}\n"
				"\n"
				"flags\n"
				"{\n"
				"\"MultiProcessorCompile\"\n"
				"}\n"
				"\n"
				"outputdir = \"%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}\"\n"
				"\n"
				"project \"" + name + "\"\n"
				"kind \"SharedLib\"\n"
				"language \"C#\"\n"
				"dotnetframework \"4.7.2\"\n"
				"\n"
				"targetdir(\"Binaries\")\n"
				"objdir(\"Project Intermediates/" + name + "\")\n"
				"\n"
				"files\n"
				"{\n"
				"\"" + scriptsPath + "/**.cs\", \n"
				"}\n"
				"\n"
				"links\n"
				"{\n"
				"\"Flora-ScriptCore\"\n"
				"}\n"
				"\n"
				"filter \"configurations:Debug\"\n"
				"optimize \"Off\"\n"
				"symbols \"Default\"\n"
				"\n"
				"filter \"configurations:Release\"\n"
				"optimize \"On\"\n"
				"symbols \"Default\"\n"
				"\n"
				"filter \"configurations:Dist\"\n"
				"optimize \"Full\"\n"
				"symbols \"Off\"\n"
				"\n"
				"group \"Flora\"\n"
				"include(FloraRootDir .. \"/Flora-ScriptCore\")\n"
				"group \"\"\n";
			premake << filecontent << std::endl;
			premake.close();
		} else {
			FL_CORE_ERROR("Failed to create premake file");
			return false;
		}

		// generate VSproj
		wchar_t originalDir[MAX_PATH];
		GetCurrentDirectoryW(MAX_PATH, originalDir);
		const wchar_t* bashdir = L"scripting";
		BOOL changeDirResult = SetCurrentDirectoryW(bashdir);
		if (!changeDirResult) {
			FL_CORE_ERROR("Failed to set up Visual Studio Project");
			return false;
		}
		const char* command = "Setup.bat";
		int result = std::system(command);
		if (result != 0) {
			FL_CORE_ERROR("Failed to set up Visual Studio Project");
			return false;
		}
		changeDirResult = SetCurrentDirectoryW(originalDir);
		if (!changeDirResult) {
			FL_CORE_ERROR("Failed to revert to original working directory");
			return false;
		}

		return true;
	}

	bool Project::RegenerateScriptingProject() {
		std::string name = s_ActiveProject->GetConfig().Name;
		std::string scriptsPath = (Project::GetAssetDirectory() / "Scripts").string();
		FL_CORE_INFO(scriptsPath);
		size_t pos = 0;
		while ((pos = scriptsPath.find("\\", pos)) != std::string::npos) {
			scriptsPath.replace(pos, 1, "/");
			pos += 1;
		}

		// create custom premake
		std::ofstream premake("scripting/premake5.lua");
		if (premake.is_open()) {
			std::string filecontent =
				"local FloraRootDir = \'../../\'\n"
				"include(FloraRootDir .. \"/vendor/premake/premake_customization/solution_items.lua\")\n"
				"\n"
				"workspace \"" + name + "\"\n"
				"architecture \"x86_64\"\n"
				"startproject \"" + name + "\"\n"
				"\n"
				"configurations\n"
				"{\n"
				"\"Debug\",\n"
				"\"Release\",\n"
				"\"Dist\"\n"
				"}\n"
				"\n"
				"flags\n"
				"{\n"
				"\"MultiProcessorCompile\"\n"
				"}\n"
				"\n"
				"outputdir = \"%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}\"\n"
				"\n"
				"project \"" + name + "\"\n"
				"kind \"SharedLib\"\n"
				"language \"C#\"\n"
				"dotnetframework \"4.7.2\"\n"
				"\n"
				"targetdir(\"Binaries\")\n"
				"objdir(\"Project Intermediates/" + name + "\")\n"
				"\n"
				"files\n"
				"{\n"
				"\"" + scriptsPath + "/**.cs\", \n"
				"}\n"
				"\n"
				"links\n"
				"{\n"
				"\"Flora-ScriptCore\"\n"
				"}\n"
				"\n"
				"filter \"configurations:Debug\"\n"
				"optimize \"Off\"\n"
				"symbols \"Default\"\n"
				"\n"
				"filter \"configurations:Release\"\n"
				"optimize \"On\"\n"
				"symbols \"Default\"\n"
				"\n"
				"filter \"configurations:Dist\"\n"
				"optimize \"Full\"\n"
				"symbols \"Off\"\n"
				"\n"
				"group \"Flora\"\n"
				"include(FloraRootDir .. \"/Flora-ScriptCore\")\n"
				"group \"\"\n";
			premake << filecontent << std::endl;
			premake.close();
		}
		else {
			FL_CORE_ERROR("Failed to create premake file");
			return false;
		}

		// generate VSproj
		wchar_t originalDir[MAX_PATH];
		GetCurrentDirectoryW(MAX_PATH, originalDir);
		const wchar_t* bashdir = L"scripting";
		BOOL changeDirResult = SetCurrentDirectoryW(bashdir);
		if (!changeDirResult) {
			FL_CORE_ERROR("Failed to set up Visual Studio Project");
			return false;
		}
		const char* command = "Setup.bat";
		int result = std::system(command);
		if (result != 0) {
			FL_CORE_ERROR("Failed to set up Visual Studio Project");
			return false;
		}
		changeDirResult = SetCurrentDirectoryW(originalDir);
		if (!changeDirResult) {
			FL_CORE_ERROR("Failed to revert to original working directory");
			return false;
		}

		return true;
	}
	#endif

	Label* Project::GetLabel(std::string label) {
		if (LabelExists(label)) {
			return m_Labels[label];
		}
		return nullptr;
	}

	bool Project::AddLabel(std::string label) {
		if (!LabelExists(label)) {
			m_Labels[label] = new Label(label);
			return true;
		}
		return false;
	}

	bool Project::RemoveLabel(std::string label) {
		if (LabelExists(label)) {
			m_Labels.erase(label);
			return true;
		}
		return false;
	}

	bool Project::LabelExists(std::string label) {
		return m_Labels.find(label) != m_Labels.end();
	}
}