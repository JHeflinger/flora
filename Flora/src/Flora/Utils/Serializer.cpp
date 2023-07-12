#include "flpch.h"
#include "Flora/Utils/Serializer.h"
#include "Flora/Scene/Entity.h"
#include "Flora/Scene/Components.h"
#include "Flora/Scripting/ScriptEngine.h"
#include <fstream>
#include <filesystem>

// temp until better native scripting system
#include "../../Fauna/assets/scripts/MasterNativeScript.h"

namespace YAML {
	template<>
	struct convert<glm::vec2> {
		static Node encode(const glm::vec2& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs) {
			if (!node.IsSequence() || node.size() != 2)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

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
			out << YAML::Key << "Visible Borders" << YAML::Value << cameraComponent.ShowBorder;
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

		if (entity.HasComponent<CircleRendererComponent>()) {
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap;
			auto& circleRendererComponent = entity.GetComponent<CircleRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << circleRendererComponent.Color;
			out << YAML::Key << "Radius" << YAML::Value << circleRendererComponent.Radius;
			out << YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << circleRendererComponent.Fade;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<ScriptComponent>()) {
			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap;
			auto& scriptComponent = entity.GetComponent<ScriptComponent>();
			out << YAML::Key << "ClassName" << YAML::Value << scriptComponent.ClassName;
			
			// fields
			Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(scriptComponent.ClassName);
			const auto& fields = entityClass->GetFields();
			if (fields.size() > 0) {
				out << YAML::Key << "ScriptFields" << YAML::Value;
				auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);
				out << YAML::BeginSeq;
				for (const auto& [name, field] : fields) {
					if (entityFields.find(name) == entityFields.end())
						continue;

					out << YAML::BeginMap;
					out << YAML::Key << "Name" << YAML::Value << name;
					out << YAML::Key << "Type" << YAML::Value << Utils::ScriptFieldTypeToString(field.Type);

					ScriptFieldInstance& scriptField = entityFields.at(name);
					#define FIELD_TYPE(FieldType, Type) case ScriptFieldType::FieldType:\
															out << YAML::Key << "Data" << YAML::Value << scriptField.GetValue<Type>();\
															break;
					switch (field.Type) {
						FIELD_TYPE(Float, float);
					}
					#undef FIELD_TYPE
					
					out << YAML::EndMap;
				}
				out << YAML::EndSeq;
			}
			
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

		if (entity.HasComponent<RigidBody2DComponent>()) {
			out << YAML::Key << "RigidBody2DComponent";
			auto& rigidBody2DComponent = entity.GetComponent<RigidBody2DComponent>();
			out << YAML::BeginMap;
			out << YAML::Key << "Type" << YAML::Value << (int)(rigidBody2DComponent.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rigidBody2DComponent.FixedRotation;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<BoxCollider2DComponent>()) {
			out << YAML::Key << "BoxCollider2DComponent";
			auto& boxCollider2DComponent = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::BeginMap;
			out << YAML::Key << "Offset" << YAML::Value << boxCollider2DComponent.Offset;
			out << YAML::Key << "Size" << YAML::Value << boxCollider2DComponent.Size;
			out << YAML::Key << "Density" << YAML::Value << boxCollider2DComponent.Density;
			out << YAML::Key << "Friction" << YAML::Value << boxCollider2DComponent.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << boxCollider2DComponent.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << boxCollider2DComponent.RestitutionThreshold;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CircleCollider2DComponent>()) {
			out << YAML::Key << "CircleCollider2DComponent";
			auto& circleCollider2DComponent = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::BeginMap;
			out << YAML::Key << "Offset" << YAML::Value << circleCollider2DComponent.Offset;
			out << YAML::Key << "Radius" << YAML::Value << circleCollider2DComponent.Radius;
			out << YAML::Key << "Density" << YAML::Value << circleCollider2DComponent.Density;
			out << YAML::Key << "Friction" << YAML::Value << circleCollider2DComponent.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << circleCollider2DComponent.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << circleCollider2DComponent.RestitutionThreshold;
			out << YAML::EndMap;
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
		out << YAML::Key << "Physics";
		out << YAML::BeginMap;
		out << YAML::Key << "Gravity" << YAML::Value << scene->GetGravity();
		out << YAML::Key << "VelocityIterations" << YAML::Value << scene->GetVelocityIterations();
		out << YAML::Key << "PositionIterations" << YAML::Value << scene->GetPositionIterations();
		out << YAML::EndMap;
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

		scene->SetGravity(data["Physics"]["Gravity"].as<float>());
		scene->SetVelocityIterations(data["Physics"]["VelocityIterations"].as<int32_t>());
		scene->SetPositionIterations(data["Physics"]["PositionIterations"].as<int32_t>());

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
					cc.ShowBorder = cameraComponent["Visible Borders"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent) {
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
					std::string texturePath = spriteRendererComponent["Path"].as<std::string>();
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

				auto circleRendererComponent = entity["CircleRendererComponent"];
				if (circleRendererComponent) {
					auto& crc = deserializedEntity.AddComponent<CircleRendererComponent>();
					crc.Color = circleRendererComponent["Color"].as<glm::vec4>();
					crc.Thickness = circleRendererComponent["Thickness"].as<float>();
					crc.Fade = circleRendererComponent["Fade"].as<float>();
					crc.Radius = circleRendererComponent["Radius"].as<float>();
				}

				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent) {
					auto& sc = deserializedEntity.AddComponent<ScriptComponent>();
					sc.ClassName = scriptComponent["ClassName"].as<std::string>();

					// fields
					auto scriptFields = scriptComponent["ScriptFields"];
					if (scriptFields) {
						Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(sc.ClassName);
						const auto& fields = entityClass->GetFields();
						auto& entityFields = ScriptEngine::GetScriptFieldMap(deserializedEntity);
						for (auto scriptField : scriptFields) {
							std::string name = scriptField["Name"].as<std::string>();
							ScriptFieldType type = Utils::ScriptFieldTypeFromSting(scriptField["Type"].as<std::string>());
							ScriptFieldInstance& fieldInstance = entityFields[name];
							fieldInstance.Field = fields.at(name);

							#define FIELD_DATA(FieldType, Type) case ScriptFieldType::FieldType:\
															Type data = scriptField["Data"].as<Type>();\
															fieldInstance.SetValue(data);\
															break;
							switch (type) {
								FIELD_DATA(Float, float);
							}

							#undef FIELD_DATA
						}
					}
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

				auto rigidBody2DComponent = entity["RigidBody2DComponent"];
				if (rigidBody2DComponent) {
					auto& rb2dc = deserializedEntity.AddComponent<RigidBody2DComponent>();
					rb2dc.Type = (RigidBody2DComponent::BodyType)rigidBody2DComponent["Type"].as<int>();
					rb2dc.FixedRotation = rigidBody2DComponent["FixedRotation"].as<bool>();
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent) {
					auto& bc2dc = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					bc2dc.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					bc2dc.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
					bc2dc.Density = boxCollider2DComponent["Density"].as<float>();
					bc2dc.Friction = boxCollider2DComponent["Friction"].as<float>();
					bc2dc.Restitution = boxCollider2DComponent["Restitution"].as<float>();
					bc2dc.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
				}

				auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
				if (circleCollider2DComponent) {
					auto& cc2dc = deserializedEntity.AddComponent<CircleCollider2DComponent>();
					cc2dc.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
					cc2dc.Radius = circleCollider2DComponent["Radius"].as<float>();
					cc2dc.Density = circleCollider2DComponent["Density"].as<float>();
					cc2dc.Friction = circleCollider2DComponent["Friction"].as<float>();
					cc2dc.Restitution = circleCollider2DComponent["Restitution"].as<float>();
					cc2dc.RestitutionThreshold = circleCollider2DComponent["RestitutionThreshold"].as<float>();
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