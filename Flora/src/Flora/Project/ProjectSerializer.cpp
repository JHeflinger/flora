#include "flpch.h"
#include "ProjectSerializer.h"
#include <yaml-cpp/yaml.h>

namespace Flora {

	ProjectSerializer::ProjectSerializer(Ref<Project> project)
		: m_Project(project) {
	}

	bool ProjectSerializer::Serialize(const std::filesystem::path& filepath) {
		const auto& config = m_Project->GetConfig();
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Project" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << config.Name;
		out << YAML::Key << "StartScene" << YAML::Value << config.StartScene.string();
		out << YAML::Key << "AssetDirectory" << YAML::Value << config.AssetDirectory.string();
		out << YAML::EndMap;
		out << YAML::EndMap;
		std::ofstream fout(filepath);
		fout << out.c_str();
		return true;
	}

	bool ProjectSerializer::Deserialize(const std::filesystem::path& filepath) {
		auto& config = m_Project->GetConfig();
		YAML::Node data;
		try {
			data = YAML::LoadFile(filepath.string());
		} catch (YAML::ParserException e) {
			FL_CORE_ERROR("Failed to load project file '{0}'\n     {1}", filepath, e.what());
			return false;
		}

		auto projectNode = data["Project"];
		if (!projectNode)
			return false;

		config.Name = projectNode["Name"].as<std::string>();
		config.StartScene = projectNode["StartScene"].as<std::string>();
		config.AssetDirectory = projectNode["AssetDirectory"].as<std::string>();
		return true;
	}

}