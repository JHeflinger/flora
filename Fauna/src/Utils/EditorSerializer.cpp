#include "flpch.h"
#include "EditorSerializer.h"
#include "Flora/Utils/Serializer.h"

namespace Flora {
	std::string EditorSerializer::Serialize(Ref<EditorParams> params) {
		int selectedEntity = -1;
		if (params->SelectedEntity)
			selectedEntity = (int)(uint32_t)params->SelectedEntity;

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene Filepath" << YAML::Value << params->ActiveScene->GetSceneFilepath();
		out << YAML::Key << "Selected Entity" << YAML::Value << selectedEntity;
		out << YAML::Key << "Current Gizmo" << YAML::Value << params->GizmoType;
		out << YAML::Key << "Editor Camera Type" << YAML::Value << params->EditorCamera.GetCameraTypeString();

		out << YAML::Key << "General Camera Settings" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Position" << YAML::Value << params->EditorCamera.GetPosition();
		out << YAML::Key << "Focal Point" << YAML::Value << params->EditorCamera.GetFocalPoint();
		out << YAML::EndMap;

		out << YAML::Key << "Perspective Camera Settings" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Distance" << YAML::Value << params->EditorCamera.GetDistance();
		out << YAML::Key << "Pitch" << YAML::Value << params->EditorCamera.GetPitch();
		out << YAML::Key << "Yaw" << YAML::Value << params->EditorCamera.GetYaw();
		out << YAML::EndMap;

		out << YAML::Key << "Orthographic Camera Settings" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Size" << YAML::Value << params->EditorCamera.GetOrthographicSize();
		out << YAML::EndMap;

		out << YAML::Key << "Closed Panels" << YAML::Value;
		out << YAML::BeginSeq;
		for (int i = 0; i < params->ClosedPanels.size(); i++)
			out << params->ClosedPanels[i];
		out << YAML::EndSeq;

		out << YAML::EndMap;

		return std::string(out.c_str());
	}

	bool EditorSerializer::Deserialize(Ref<EditorParams> params, const std::string& filepath) {
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene Filepath"] ||
			!data["Current Gizmo"] ||
			!data["Selected Entity"] ||
			!data["Editor Camera Type"] ||
			!data["General Camera Settings"] ||
			!data["Perspective Camera Settings"] ||
			!data["Orthographic Camera Settings"] ||
			!data["Closed Panels"])
			return false;

		// set up saved scene
		std::string sceneFilepath = data["Scene Filepath"].as<std::string>();
		params->ActiveScene->SetSceneFilepath(sceneFilepath);
		bool success = Serializer::DeserializeScene(params->ActiveScene, sceneFilepath);

		// set closed panels
		params->ClosedPanels = data["Closed Panels"].as<std::vector<std::string>>();

		// set camera settings
		params->EditorCamera.SetCameraTypeWithString(data["Editor Camera Type"].as<std::string>());
		params->EditorCamera.SetPosition(Serializer::GetNodeAsVec3(data["General Camera Settings"]["Position"]));
		params->EditorCamera.SetFocalPoint(Serializer::GetNodeAsVec3(data["General Camera Settings"]["Focal Point"]));
		params->EditorCamera.SetDistance(data["Perspective Camera Settings"]["Distance"].as<float>());
		params->EditorCamera.SetPitch(data["Perspective Camera Settings"]["Pitch"].as<float>());
		params->EditorCamera.SetYaw(data["Perspective Camera Settings"]["Yaw"].as<float>());
		params->EditorCamera.SetOrthographicSize(data["Orthographic Camera Settings"]["Size"].as<float>());

		// set current gizmo
		params->GizmoType = data["Current Gizmo"].as<int>();

		// set selected entity
		int entityID = data["Selected Entity"].as<int>();
		if (entityID >= 0 && params->ActiveScene->EntityExists((uint32_t)entityID))
			params->SelectedEntity = params->ActiveScene->GetEntityFromID((uint32_t)entityID);

		return success;
	}
}