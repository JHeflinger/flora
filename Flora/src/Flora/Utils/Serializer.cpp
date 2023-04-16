#include "flpch.h"
#include "Flora/Utils/Serializer.h"
#include "Flora/Scene/Entity.h"
#include "Flora/Scene/Components.h"
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <filesystem>

// temp until better native scripting system
#include "../../Fauna/assets/scripts/MasterNativeScript.h"

namespace YAML {
	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Flora {
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity) {
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity;

		if (entity.HasComponent<TagComponent>()) {
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>()) {
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;
			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())	{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;
			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;
			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap;
			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())	{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;
			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
			out << YAML::Key << "Path" << YAML::Value << spriteRendererComponent.Path;
			out << YAML::Key << "Filename" << YAML::Value << spriteRendererComponent.Filename;
			out << YAML::Key << "Type" << YAML::Value << (int)spriteRendererComponent.Type;
			out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;
			out << YAML::Key << "Rows" << YAML::Value << spriteRendererComponent.Rows;
			out << YAML::Key << "Columns" << YAML::Value << spriteRendererComponent.Columns;
			out << YAML::Key << "RowCoordinate" << YAML::Value << spriteRendererComponent.RowCoordinate;
			out << YAML::Key << "ColumnCoordinate" << YAML::Value << spriteRendererComponent.ColumnCoordinate;
			out << YAML::Key << "SubtextureWidth" << YAML::Value << spriteRendererComponent.SubtextureWidth;
			out << YAML::Key << "SubtextureHeight" << YAML::Value << spriteRendererComponent.SubtextureHeight;
			out << YAML::Key << "Frames" << YAML::Value << spriteRendererComponent.Frames;
			out << YAML::Key << "StartFrame" << YAML::Value << spriteRendererComponent.StartFrame;
			out << YAML::Key << "EndFrame" << YAML::Value << spriteRendererComponent.EndFrame;
			out << YAML::Key << "CurrentFrame" << YAML::Value << spriteRendererComponent.CurrentFrame;
			out << YAML::Key << "FPS" << YAML::Value << spriteRendererComponent.FPS;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<NativeScriptComponent>()) {
			out << YAML::Key << "NativeScriptComponent";
			out << YAML::BeginMap;
			auto& nativeScriptComponent = entity.GetComponent<NativeScriptComponent>();
			out << YAML::Key << "Path" << YAML::Value << nativeScriptComponent.Path;
			out << YAML::Key << "Filename" << YAML::Value << nativeScriptComponent.Filename;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void Serializer::SerializeScene(Ref<Scene>& scene, const std::string& filepath) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		scene->m_Registry.each([&](auto entityID) {
			Entity entity = { entityID, scene.get() };
			if (!entity) return;
			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void Serializer::SerializeRuntime() {
		// not implemented yet!
		FL_CORE_ASSERT(false);
	}

	void Serializer::SerializeEditor(Ref<EditorParams> params, const std::string& filepath) {
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

		out << YAML::EndMap;
		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool Serializer::DeserializeScene(Ref<Scene>& scene, const std::string& filepath) {
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		FL_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {
				uint64_t uuid = entity["Entity"].as<uint32_t>(); // TODO

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				FL_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = scene->CreateEntity(uuid, name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)	{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent) {
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent) {
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
					std::string texturePath = spriteRendererComponent["Path"].as<std::string>();
					if (texturePath != "NULL")
						src.Texture = Texture2D::Create(spriteRendererComponent["Path"].as<std::string>());
					src.Type = (SpriteRendererComponent::SpriteType)spriteRendererComponent["Type"].as<int>();
					src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
					src.Rows = spriteRendererComponent["Rows"].as<int>();
					src.Columns = spriteRendererComponent["Columns"].as<int>();
					src.RowCoordinate = spriteRendererComponent["RowCoordinate"].as<int>();
					src.ColumnCoordinate = spriteRendererComponent["ColumnCoordinate"].as<int>();
					src.SubtextureWidth = spriteRendererComponent["SubtextureWidth"].as<float>();
					src.SubtextureHeight = spriteRendererComponent["SubtextureHeight"].as<float>();
					src.Frames = spriteRendererComponent["Frames"].as<int>();
					src.StartFrame = spriteRendererComponent["StartFrame"].as<int>();
					src.EndFrame = spriteRendererComponent["EndFrame"].as<int>();
					src.CurrentFrame = spriteRendererComponent["CurrentFrame"].as<int>();
					src.FPS = spriteRendererComponent["FPS"].as<int>();
					src.Path = spriteRendererComponent["Path"].as<std::string>();
					src.Filename = spriteRendererComponent["Filename"].as<std::string>();
				}

				auto nativeScriptComponent = entity["NativeScriptComponent"];
				if (nativeScriptComponent) {
					auto& nsc = deserializedEntity.AddComponent<NativeScriptComponent>();
					nsc.Path = nativeScriptComponent["Path"].as<std::string>();
					nsc.Filename = nativeScriptComponent["Filename"].as<std::string>();
					BindScriptToComponent(nsc, std::filesystem::path(nsc.Path).filename().stem().string());
				}
			}
		}

		return true;
	}

	bool Serializer::DeserializeRuntime() {
		// not implemented yet!
		FL_CORE_ASSERT(false);
		return false;
	}

	bool Serializer::DeserializeEditor(Ref<EditorParams> params, const std::string& filepath) {
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
			!data["Orthographic Camera Settings"])
			return false;

		// set up saved scene
		std::string sceneFilepath = data["Scene Filepath"].as<std::string>();
		params->ActiveScene->SetSceneFilepath(sceneFilepath);
		bool success = DeserializeScene(params->ActiveScene, sceneFilepath);

		// set camera settings
		params->EditorCamera.SetCameraTypeWithString(data["Editor Camera Type"].as<std::string>());
		params->EditorCamera.SetPosition(data["General Camera Settings"]["Position"].as<glm::vec3>());
		params->EditorCamera.SetFocalPoint(data["General Camera Settings"]["Focal Point"].as<glm::vec3>());
		params->EditorCamera.SetDistance(data["Perspective Camera Settings"]["Distance"].as<float>());
		params->EditorCamera.SetPitch(data["Perspective Camera Settings"]["Pitch"].as<float>());
		params->EditorCamera.SetYaw(data["Perspective Camera Settings"]["Yaw"].as<float>());
		params->EditorCamera.SetOrthographicSize(data["Orthographic Camera Settings"]["Size"].as<float>());

		// set current gizmo
		params->GizmoType = data["Current Gizmo"].as<int>();

		// set selected entity
		int entityID = data["Selected Entity"].as<int>();
		if (entityID >= 0)
			params->SelectedEntity = params->ActiveScene->GetEntityFromID((uint32_t)entityID);

		return success;
	}
}