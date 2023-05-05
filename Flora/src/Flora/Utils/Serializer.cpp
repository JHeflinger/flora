#include "flpch.h"
#include "Flora/Utils/Serializer.h"
#include "Flora/Scene/Entity.h"
#include "Flora/Scene/Components.h"
#include <fstream>
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

			// accomodate for variances in sprite animation for comparisons
			if (spriteRendererComponent.Type != SpriteRendererComponent::SpriteType::ANIMATION) {
				out << YAML::Key << "RowCoordinate" << YAML::Value << spriteRendererComponent.RowCoordinate;
				out << YAML::Key << "ColumnCoordinate" << YAML::Value << spriteRendererComponent.ColumnCoordinate;
			} else {
				out << YAML::Key << "RowCoordinate" << YAML::Value << 1;
				out << YAML::Key << "ColumnCoordinate" << YAML::Value << 1;
			}

			out << YAML::Key << "SubtextureWidth" << YAML::Value << spriteRendererComponent.SubtextureWidth;
			out << YAML::Key << "SubtextureHeight" << YAML::Value << spriteRendererComponent.SubtextureHeight;
			out << YAML::Key << "Frames" << YAML::Value << spriteRendererComponent.Frames;
			out << YAML::Key << "StartFrame" << YAML::Value << spriteRendererComponent.StartFrame;
			out << YAML::Key << "EndFrame" << YAML::Value << spriteRendererComponent.EndFrame;
			out << YAML::Key << "CurrentFrame" << YAML::Value << spriteRendererComponent.StartFrame;
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

		if (entity.HasComponent<ScriptManagerComponent>()) {
			out << YAML::Key << "ScriptManagerComponent";
			auto& scriptManager = entity.GetComponent<ScriptManagerComponent>();
			out << YAML::BeginMap;
			out << YAML::Key << "NativeScripts";
			out << YAML::BeginSeq;
			for (int i = 0; i < scriptManager.NativeScripts.size(); i++) {
				out << YAML::BeginMap;
				out << YAML::Key << "Path" << YAML::Value << scriptManager.NativeScripts[i].Path;
				out << YAML::Key << "Filename" << YAML::Value << scriptManager.NativeScripts[i].Filename;
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<ParentComponent>()) {
			out << YAML::Key << "ParentComponent";
			out << YAML::BeginMap;
			auto& parentComponent = entity.GetComponent<ParentComponent>();
			out << YAML::Key << "Parent" << YAML::Value << (uint32_t)parentComponent.Parent;
			out << YAML::Key << "InheritAll" << YAML::Value << parentComponent.InheritAll;
			out << YAML::Key << "InheritTransform" << YAML::Value << parentComponent.InheritTransform;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<ChildComponent>()) {
			out << YAML::Key << "ChildComponent";
			auto& childComponent = entity.GetComponent<ChildComponent>();
			out << YAML::BeginSeq;
			for (int i = 0; i < childComponent.Children.size(); i++)
				out << (uint32_t)childComponent.Children[i];
			out << YAML::EndSeq;
		}

		out << YAML::EndMap;
	}

	void Serializer::SerializeFile(const std::string content, const std::string& filepath) {
		std::ofstream fout(filepath);
		fout << content.c_str();
	}

	std::string Serializer::SerializeScene(Ref<Scene>& scene) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << scene->GetSceneName();
		out << YAML::Key << "Primary Camera" << YAML::Value << scene->GetPrimaryCamera();
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		scene->m_Registry.each([&](auto entityID) {
			Entity entity = { entityID, scene.get() };
			if (!entity) return;
			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		return std::string(out.c_str());
	}

	std::string Serializer::SerializeRuntime() {
		// not implemented yet!
		FL_CORE_ASSERT(false);
		return "";
	}

	bool Serializer::DeserializeScene(Ref<Scene>& scene, const std::string& filepath) {
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		scene->SetSceneName(sceneName);
		FL_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		scene->SetPrimaryCamera(data["Primary Camera"].as<int>());

		auto entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {
				uint64_t uuid = entity["Entity"].as<uint32_t>(); // TODO

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

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

				auto parentComponent = entity["ParentComponent"];
				if (parentComponent) {
					auto& pc = deserializedEntity.AddComponent<ParentComponent>();
					pc.Parent = Entity{ (entt::entity)parentComponent["Parent"].as<uint32_t>(), scene.get() };
					pc.InheritAll = parentComponent["InheritAll"].as<bool>();
					pc.InheritTransform = parentComponent["InheritTransform"].as<bool>();
				}

				auto childComponent = entity["ChildComponent"];
				if (childComponent) {
					auto& cc = deserializedEntity.AddComponent<ChildComponent>();
					for (auto child : childComponent)
						cc.AddChild(Entity{ (entt::entity)child.as<uint32_t>(), scene.get() });
				}

				auto scriptManager = entity["ScriptManagerComponent"];
				if (scriptManager) {
					auto& smc = deserializedEntity.AddComponent<ScriptManagerComponent>();
					auto nativeScripts = scriptManager["NativeScripts"];
					for (auto nsc : nativeScripts) {
						NativeScriptComponent newNativeScript;
						newNativeScript.Path = nsc["Path"].as<std::string>();
						newNativeScript.Filename = nsc["Filename"].as<std::string>();
						BindScriptToComponent(newNativeScript, std::filesystem::path(newNativeScript.Path).filename().stem().string());
						smc.NativeScripts.emplace_back(newNativeScript);
					}
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

	bool Serializer::IsSceneSaved(Ref<Scene>& scene) {
		// forgot to make it so time resets wne u save btw
		std::ifstream file(scene->m_SceneFilepath);
		if (file.is_open()) {
			std::string newSceneContent = SerializeScene(scene);
			std::string oldSceneContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			return newSceneContent == oldSceneContent;
		}
		return false;
	}

	glm::vec3 Serializer::GetNodeAsVec3(YAML::Node node) {
		return node.as<glm::vec3>();
	}

	glm::vec4 Serializer::GetNodeAsVec4(YAML::Node node) {
		return node.as<glm::vec4>();
	}

	std::vector<std::string> Serializer::GetNodeAsVecString(YAML::Node node) {
		return node.as<std::vector<std::string>>();
	}
}