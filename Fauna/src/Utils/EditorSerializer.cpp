#include "flpch.h"
#include "EditorSerializer.h"
#include "Flora/Utils/Serializer.h"
#include "Flora/Project/Project.h"
#include "Flora/Scripting/ScriptEngine.h"

namespace Flora {
	std::string EditorSerializer::Serialize(Ref<EditorParams> params) {
		int selectedEntity = -1;
		if (params->SelectedEntity)
			selectedEntity = (int)(uint32_t)params->SelectedEntity;

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Project Filepath" << YAML::Value << params->ProjectFilepath;
		out << YAML::Key << "Scene Filepath" << YAML::Value << params->ActiveScene->GetSceneFilepath();
		out << YAML::Key << "Selected Entity" << YAML::Value << selectedEntity;
		out << YAML::Key << "Current Gizmo" << YAML::Value << params->GizmoType;
		out << YAML::Key << "Editor Camera Type" << YAML::Value << params->EditorCamera.GetCameraTypeString();
		out << YAML::Key << "Visible Physics Colliders" << YAML::Value << params->VisibleColliders;

		out << YAML::Key << "General Camera Settings" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Position" << YAML::Value << params->EditorCamera.GetPosition();
		out << YAML::Key << "Focal Point" << YAML::Value << params->EditorCamera.GetFocalPoint();
		out << YAML::Key << "Camera Bound" << YAML::Value << *(params->EditorCamera.GetCameraBound());
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

		out << YAML::Key << "Stat Panel Settings" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Timeframe" << YAML::Value << params->Timeframe;
		out << YAML::Key << "Show Key" << YAML::Value << params->ShowGraphKey;
		out << YAML::Key << "Axis Scalar" << YAML::Value << params->AxisScale;
		out << YAML::Key << "Axis Minimum" << YAML::Value << params->AxisMinimum;
		out << YAML::Key << "Open Graphs" << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "DRAWCALLS" << YAML::Value << params->ShowStatMap[Stats::DRAWCALLS];
		out << YAML::Key << "QUADS" << YAML::Value << params->ShowStatMap[Stats::QUADS];
		out << YAML::Key << "CIRCLES" << YAML::Value << params->ShowStatMap[Stats::CIRCLES];
		out << YAML::Key << "LINES" << YAML::Value << params->ShowStatMap[Stats::LINES];
		out << YAML::Key << "VERTICES" << YAML::Value << params->ShowStatMap[Stats::VERTICES];
		out << YAML::Key << "INDICES" << YAML::Value << params->ShowStatMap[Stats::INDICES];
		out << YAML::Key << "FRAMETIME" << YAML::Value << params->ShowStatMap[Stats::FRAMETIME];
		out << YAML::Key << "FPS" << YAML::Value << params->ShowStatMap[Stats::FPS];
		out << YAML::Key << "LOWEST_FPS" << YAML::Value << params->ShowStatMap[Stats::LOWEST_FPS];
		out << YAML::Key << "HIGHEST_FPS" << YAML::Value << params->ShowStatMap[Stats::HIGHEST_FPS];
		out << YAML::Key << "CPU_USAGE" << YAML::Value << params->ShowStatMap[Stats::CPU_USAGE];
		out << YAML::Key << "GPU_USAGE" << YAML::Value << params->ShowStatMap[Stats::GPU_USAGE];
		out << YAML::Key << "MEM_USAGE" << YAML::Value << params->ShowStatMap[Stats::MEM_USAGE];
		out << YAML::Key << "DISK_USAGE" << YAML::Value << params->ShowStatMap[Stats::DISK_USAGE];
		out << YAML::EndMap;
		out << YAML::EndMap;

		out << YAML::EndMap;

		return std::string(out.c_str());
	}

	bool EditorSerializer::Deserialize(Ref<EditorParams> params, const std::string& filepath) {
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Project Filepath"] ||
			!data["Scene Filepath"] ||
			!data["Current Gizmo"] ||
			!data["Selected Entity"] ||
			!data["Editor Camera Type"] ||
			!data["General Camera Settings"] ||
			!data["Perspective Camera Settings"] ||
			!data["Orthographic Camera Settings"] ||
			!data["Closed Panels"] ||
			!data["Stat Panel Settings"])
			return false;

		// set up project
		std::string projectFilepath = data["Project Filepath"].as<std::string>();
		if (projectFilepath == "")
			params->Project = Project::New();
		else
			params->Project = Project::Load(projectFilepath);
		params->ProjectFilepath = projectFilepath;

		// TODO: HORRIBLE SOLUTION RIGHT NOW... REFACTOR WHEN RESTRUCTURING SCENE SET UP SYSTEM LATER
		if (!ScriptEngine::IsInitialized() && params->ProjectFilepath != "")
			ScriptEngine::Init();

		// set up saved scene if the project is valid
		bool success = false;
		if (params->ProjectFilepath != "") {
			std::string sceneFilepath = data["Scene Filepath"].as<std::string>();
			params->ActiveScene->SetSceneFilepath(sceneFilepath);
			success = Serializer::DeserializeScene(params->ActiveScene, sceneFilepath);
		}

		// set closed panels
		params->ClosedPanels = data["Closed Panels"].as<std::vector<std::string>>();

		// set visible colliders
		params->VisibleColliders = data["Visible Physics Colliders"].as<bool>();

		// set camera settings
		params->EditorCamera.SetCameraTypeWithString(data["Editor Camera Type"].as<std::string>());
		params->EditorCamera.SetPosition(Serializer::GetNodeAsVec3(data["General Camera Settings"]["Position"]));
		params->EditorCamera.SetFocalPoint(Serializer::GetNodeAsVec3(data["General Camera Settings"]["Focal Point"]));
		*(params->EditorCamera.GetCameraBound()) = data["General Camera Settings"]["Camera Bound"].as<bool>();
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

		// set up stats panel
		params->Timeframe = data["Stat Panel Settings"]["Timeframe"].as<float>();
		params->ShowGraphKey = data["Stat Panel Settings"]["Show Key"].as<bool>();
		params->AxisMinimum = data["Stat Panel Settings"]["Axis Minimum"].as<float>();
		params->AxisScale = data["Stat Panel Settings"]["Axis Scalar"].as<float>();
		params->ShowStatMap[Stats::DRAWCALLS] = data["Stat Panel Settings"]["Open Graphs"]["DRAWCALLS"].as<bool>();
		params->ShowStatMap[Stats::QUADS] = data["Stat Panel Settings"]["Open Graphs"]["QUADS"].as<bool>();
		params->ShowStatMap[Stats::CIRCLES] = data["Stat Panel Settings"]["Open Graphs"]["CIRCLES"].as<bool>();
		params->ShowStatMap[Stats::LINES] = data["Stat Panel Settings"]["Open Graphs"]["LINES"].as<bool>();
		params->ShowStatMap[Stats::VERTICES] = data["Stat Panel Settings"]["Open Graphs"]["VERTICES"].as<bool>();
		params->ShowStatMap[Stats::INDICES] = data["Stat Panel Settings"]["Open Graphs"]["INDICES"].as<bool>();
		params->ShowStatMap[Stats::FRAMETIME] = data["Stat Panel Settings"]["Open Graphs"]["FRAMETIME"].as<bool>();
		params->ShowStatMap[Stats::FPS] = data["Stat Panel Settings"]["Open Graphs"]["FPS"].as<bool>();
		params->ShowStatMap[Stats::LOWEST_FPS] = data["Stat Panel Settings"]["Open Graphs"]["LOWEST_FPS"].as<bool>();
		params->ShowStatMap[Stats::HIGHEST_FPS] = data["Stat Panel Settings"]["Open Graphs"]["HIGHEST_FPS"].as<bool>();
		params->ShowStatMap[Stats::CPU_USAGE] = data["Stat Panel Settings"]["Open Graphs"]["CPU_USAGE"].as<bool>();
		params->ShowStatMap[Stats::GPU_USAGE] = data["Stat Panel Settings"]["Open Graphs"]["GPU_USAGE"].as<bool>();
		params->ShowStatMap[Stats::MEM_USAGE] = data["Stat Panel Settings"]["Open Graphs"]["MEM_USAGE"].as<bool>();
		params->ShowStatMap[Stats::DISK_USAGE] = data["Stat Panel Settings"]["Open Graphs"]["DISK_USAGE"].as<bool>();

		return success;
	}
}