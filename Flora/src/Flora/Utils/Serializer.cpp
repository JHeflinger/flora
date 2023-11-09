#include "flpch.h"
#include "Serializer.h"
#include "Flora/Scene/Entity.h"
#include "Flora/Scene/Components.h"
#include "Flora/Scripting/ScriptEngine.h"
#include "Flora/Project/Project.h"

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
	static void SerializeEntityYAML(YAML::Emitter& out, Entity entity, bool toplevel = false) {
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
			out << YAML::Key << "Visible" << YAML::Value << spriteRendererComponent.Visible;
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
			out << YAML::Key << "Path" << YAML::Value << spriteRendererComponent.Path;
			out << YAML::Key << "Filename" << YAML::Value << spriteRendererComponent.Filename;
			out << YAML::Key << "Type" << YAML::Value << (int)spriteRendererComponent.Type;
			out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;
			out << YAML::Key << "Rows" << YAML::Value << spriteRendererComponent.Rows;
			out << YAML::Key << "Columns" << YAML::Value << spriteRendererComponent.Columns;
			out << YAML::Key << "Paused" << YAML::Value << spriteRendererComponent.Paused;

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
			if (entityClass) {
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
							FIELD_TYPE(Vector2, glm::vec2);
							FIELD_TYPE(Vector3, glm::vec3);
							FIELD_TYPE(Vector4, glm::vec4);
							FIELD_TYPE(Int, int);
							FIELD_TYPE(UInt, uint32_t);
							FIELD_TYPE(Bool, bool);
							FIELD_TYPE(Double, double);
							FIELD_TYPE(Short, uint16_t);
							FIELD_TYPE(Byte, uint16_t); //uint8_t makes it treated like a char for some reason, YAML lib bug maybe?
						}

						#undef FIELD_TYPE

						out << YAML::EndMap;
					}
					out << YAML::EndSeq;
				}
			}
			
			out << YAML::EndMap;
		}

		if (entity.HasComponent<ScriptManagerComponent>()) {
			out << YAML::Key << "ScriptManagerComponent";
			auto& scriptManager = entity.GetComponent<ScriptManagerComponent>();
			//TODO
		}

		if (entity.HasComponent<ParentComponent>() && !toplevel) {
			out << YAML::Key << "ParentComponent";
			out << YAML::BeginMap;
			auto& parentComponent = entity.GetComponent<ParentComponent>();
			out << YAML::Key << "Parent" << YAML::Value << (uint32_t)parentComponent.Parent;
			out << YAML::Key << "InheritAll" << YAML::Value << parentComponent.InheritAll;
			out << YAML::Key << "InheritTransform" << YAML::Value << parentComponent.InheritTransform;
			out << YAML::Key << "InheritSpriteProperties" << YAML::Value << parentComponent.InheritSpriteProperties;
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

		if (entity.HasComponent<AudioSourceComponent>()) {
			out << YAML::Key << "AudioSourceComponent";
			auto& audioSourceComponent = entity.GetComponent<AudioSourceComponent>();
			out << YAML::BeginMap;
			out << YAML::Key << "Scale" << YAML::Value << audioSourceComponent.Scale;
			out << YAML::Key << "AudioFile" << YAML::Value << audioSourceComponent.AudioFile;
			out << YAML::Key << "Loop" << YAML::Value << audioSourceComponent.Loop;
			out << YAML::Key << "Pitch" << YAML::Value << audioSourceComponent.Pitch;
			out << YAML::Key << "Gain" << YAML::Value << audioSourceComponent.Gain;
			out << YAML::Key << "Velocity" << YAML::Value << audioSourceComponent.Velocity;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<AudioListenerComponent>()) {
			out << YAML::Key << "AudioListenerComponent";
			auto& audioListenerComponent = entity.GetComponent<AudioListenerComponent>();
			out << YAML::BeginMap;
			out << YAML::Key << "Gain" << YAML::Value << audioListenerComponent.Gain;
			out << YAML::Key << "Velocity" << YAML::Value << audioListenerComponent.Velocity;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<LabelComponent>()) {
			out << YAML::Key << "LabelComponent";
			out << YAML::BeginSeq;
			for (auto label : entity.GetComponent<LabelComponent>().GetLabels())
				out << label;
			out << YAML::EndSeq;
		}

		if (entity.HasComponent<TextComponent>()) {
			out << YAML::Key << "TextComponent";
			auto& textComponent = entity.GetComponent<TextComponent>();
			out << YAML::BeginMap;
			out << YAML::Key << "TextString" << YAML::Value << textComponent.TextString;
			out << YAML::Key << "Kerning" << YAML::Value << textComponent.Kerning;
			out << YAML::Key << "LineSpacing" << YAML::Value << textComponent.LineSpacing;
			out << YAML::Key << "Color" << YAML::Value << textComponent.Color;
			out << YAML::Key << "Translation" << YAML::Value << textComponent.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << textComponent.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << textComponent.Scale;
			out << YAML::Key << "Alignment" << YAML::Value << (int)textComponent.Alignment;
			out << YAML::Key << "FontFilePath" << YAML::Value << textComponent.FontFilePath;
			out << YAML::Key << "FontName" << YAML::Value << textComponent.FontName;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	static Entity DeserializeEntityYAML(YAML::Node& data, Ref<Scene>& scene, std::map<uint32_t, uint32_t>& uidmap) {
		uint64_t uuid = data["Entity"].as<uint32_t>(); // TODO

		std::string name;
		auto tagComponent = data["TagComponent"];
		if (tagComponent)
			name = tagComponent["Tag"].as<std::string>();

		Entity deserializedEntity;
		if (uidmap.find(uuid) != uidmap.end()) { // overriding an existing but uninitialized child entity, so initialize it and use that
			deserializedEntity = scene->GetEntityFromID(uidmap[uuid]);
			if (!deserializedEntity.HasComponent<TagComponent>())
				deserializedEntity.AddComponent<TagComponent>();
			if (!deserializedEntity.HasComponent<TransformComponent>())
				deserializedEntity.AddComponent<TransformComponent>();
			deserializedEntity.GetComponent<TagComponent>().Tag = name;
		} else {
			if (scene->EntityExists(uuid)) // this entity already exists, create a new entity and use that ID instead
				deserializedEntity = scene->CreateEntity(name);
			else // this entity doesnt exist, so this is a valid ID to use
				deserializedEntity = scene->CreateEntity(uuid, name);
			uidmap[uuid] = (uint32_t)deserializedEntity;
		}

		auto transformComponent = data["TransformComponent"];
		if (transformComponent) {
			auto& tc = deserializedEntity.GetComponent<TransformComponent>();
			tc.Translation = transformComponent["Translation"].as<glm::vec3>();
			tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
			tc.Scale = transformComponent["Scale"].as<glm::vec3>();
		}

		auto cameraComponent = data["CameraComponent"];
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

		auto spriteRendererComponent = data["SpriteRendererComponent"];
		if (spriteRendererComponent) {
			auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
			src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
			src.Visible = spriteRendererComponent["Visible"].as<bool>();
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
			src.Paused = spriteRendererComponent["Paused"].as<bool>();
		}

		auto circleRendererComponent = data["CircleRendererComponent"];
		if (circleRendererComponent) {
			auto& crc = deserializedEntity.AddComponent<CircleRendererComponent>();
			crc.Color = circleRendererComponent["Color"].as<glm::vec4>();
			crc.Thickness = circleRendererComponent["Thickness"].as<float>();
			crc.Fade = circleRendererComponent["Fade"].as<float>();
			crc.Radius = circleRendererComponent["Radius"].as<float>();
		}

		auto scriptComponent = data["ScriptComponent"];
		if (scriptComponent) {
			auto& sc = deserializedEntity.AddComponent<ScriptComponent>();
			sc.ClassName = scriptComponent["ClassName"].as<std::string>();

			// fields
			auto scriptFields = scriptComponent["ScriptFields"];
			if (scriptFields) {
				Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(sc.ClassName);
				if (entityClass) {
					const auto& fields = entityClass->GetFields();
					auto& entityFields = ScriptEngine::GetScriptFieldMap(deserializedEntity);
					for (auto scriptField : scriptFields) {
						std::string name = scriptField["Name"].as<std::string>();
						ScriptFieldType type = Utils::ScriptFieldTypeFromSting(scriptField["Type"].as<std::string>());
						ScriptFieldInstance& fieldInstance = entityFields[name];
						if (fields.find(name) == fields.end()) continue;
						fieldInstance.Field = fields.at(name);

						#define FIELD_DATA(FieldType, Type) case ScriptFieldType::FieldType:\
															{Type data = scriptField["Data"].as<Type>();\
															fieldInstance.SetValue<Type>(data);\
															break;}
						switch (type) {
							FIELD_DATA(Float, float);
							FIELD_DATA(Vector2, glm::vec2);
							FIELD_DATA(Vector3, glm::vec3);
							FIELD_DATA(Vector4, glm::vec4);
							FIELD_DATA(Int, int);
							FIELD_DATA(UInt, uint32_t);
							FIELD_DATA(Bool, bool);
							FIELD_DATA(Double, double);
							FIELD_DATA(Short, uint16_t);
							FIELD_DATA(Byte, uint8_t);
						}

						#undef FIELD_DATA
					}
				}
				else FL_CORE_ERROR("Unable to find the class {} in the script binary", sc.ClassName);
			}
		}

		auto parentComponent = data["ParentComponent"];
		if (parentComponent) {
			auto& pc = deserializedEntity.AddComponent<ParentComponent>();
			uint32_t parentid = uidmap.find(parentComponent["Parent"].as<uint32_t>()) == uidmap.end() ? parentComponent["Parent"].as<uint32_t>() : uidmap[parentComponent["Parent"].as<uint32_t>()];
			pc.Parent = Entity{ (entt::entity)parentid, scene.get() };
			if (parentComponent["InheritAll"])
				pc.InheritAll = parentComponent["InheritAll"].as<bool>();
			if (parentComponent["InheritTransform"])
				pc.InheritTransform = parentComponent["InheritTransform"].as<bool>();
			if (parentComponent["InheritSpriteProperties"])
				pc.InheritSpriteProperties = parentComponent["InheritSpriteProperties"].as<bool>();
		}

		auto childComponent = data["ChildComponent"];
		if (childComponent) {
			auto& cc = deserializedEntity.AddComponent<ChildComponent>();
			for (auto child : childComponent)
				if (uidmap.find(child.as<uint32_t>()) != uidmap.end()) {
					cc.AddChild(scene->GetEntityFromID(child.as<uint32_t>()));
				} else {
					Entity newchild;
					if (scene->EntityExists(child.as<uint32_t>()))
						newchild = scene->CreateEntity();
					else
						newchild = scene->CreateEntity(child.as<uint32_t>());
					uidmap[child.as<uint32_t>()] = (uint32_t)newchild;
					cc.AddChild(newchild);
				}
		}

		auto scriptManager = data["ScriptManagerComponent"];
		if (scriptManager) {
			auto& smc = deserializedEntity.AddComponent<ScriptManagerComponent>();
			//TODO
		}

		auto rigidBody2DComponent = data["RigidBody2DComponent"];
		if (rigidBody2DComponent) {
			auto& rb2dc = deserializedEntity.AddComponent<RigidBody2DComponent>();
			rb2dc.Type = (RigidBody2DComponent::BodyType)rigidBody2DComponent["Type"].as<int>();
			rb2dc.FixedRotation = rigidBody2DComponent["FixedRotation"].as<bool>();
		}

		auto boxCollider2DComponent = data["BoxCollider2DComponent"];
		if (boxCollider2DComponent) {
			auto& bc2dc = deserializedEntity.AddComponent<BoxCollider2DComponent>();
			bc2dc.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
			bc2dc.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
			bc2dc.Density = boxCollider2DComponent["Density"].as<float>();
			bc2dc.Friction = boxCollider2DComponent["Friction"].as<float>();
			bc2dc.Restitution = boxCollider2DComponent["Restitution"].as<float>();
			bc2dc.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
		}

		auto circleCollider2DComponent = data["CircleCollider2DComponent"];
		if (circleCollider2DComponent) {
			auto& cc2dc = deserializedEntity.AddComponent<CircleCollider2DComponent>();
			cc2dc.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
			cc2dc.Radius = circleCollider2DComponent["Radius"].as<float>();
			cc2dc.Density = circleCollider2DComponent["Density"].as<float>();
			cc2dc.Friction = circleCollider2DComponent["Friction"].as<float>();
			cc2dc.Restitution = circleCollider2DComponent["Restitution"].as<float>();
			cc2dc.RestitutionThreshold = circleCollider2DComponent["RestitutionThreshold"].as<float>();
		}

		auto audioSourceComponent = data["AudioSourceComponent"];
		if (audioSourceComponent) {
			auto& asc = deserializedEntity.AddComponent<AudioSourceComponent>();
			asc.Scale = audioSourceComponent["Scale"].as<float>();
			asc.AudioFile = audioSourceComponent["AudioFile"].as<std::string>();
			asc.Loop = audioSourceComponent["Loop"].as<bool>();
			asc.Pitch = audioSourceComponent["Pitch"].as<float>();
			asc.Gain = audioSourceComponent["Gain"].as<float>();
			asc.Velocity = audioSourceComponent["Velocity"].as<glm::vec3>();
		}

		auto audioListenerComponent = data["AudioListenerComponent"];
		if (audioListenerComponent) {
			auto& alc = deserializedEntity.AddComponent<AudioListenerComponent>();
			alc.Gain = audioListenerComponent["Gain"].as<float>();
			alc.Velocity = audioListenerComponent["Velocity"].as<glm::vec3>();
		}

		auto labelComponent = data["LabelComponent"];
		if (labelComponent) {
			auto& lc = deserializedEntity.AddComponent<LabelComponent>();
			for (auto label : labelComponent)
				lc.AddLabel(label.as<std::string>());
		}

		auto textComponent = data["TextComponent"];
		if (textComponent) {
			auto& tc = deserializedEntity.AddComponent<TextComponent>();
			tc.TextString = textComponent["TextString"].as<std::string>();
			tc.Kerning = textComponent["Kerning"].as<float>();
			tc.LineSpacing = textComponent["LineSpacing"].as<float>();
			tc.Color = textComponent["Color"].as<glm::vec4>();
			tc.Translation = textComponent["Translation"].as<glm::vec3>();
			tc.Scale = textComponent["Scale"].as<glm::vec3>();
			tc.Rotation = textComponent["Rotation"].as<glm::vec3>();
			tc.Alignment = (FontAlignment)textComponent["Alignment"].as<int>();
			tc.FontFilePath = textComponent["FontFilePath"].as<std::string>();
			tc.FontName = textComponent["FontName"].as<std::string>();
		}

		return deserializedEntity;
	}

	static void SerializeEntityDependancy(YAML::Emitter& out, Entity entity) {
		SerializeEntityYAML(out, entity);
		if (entity.HasComponent<ChildComponent>()) {
			std::vector<Entity> children = entity.GetComponent<ChildComponent>().Children;
			for (int i = 0; i < children.size(); i++) {
				SerializeEntityDependancy(out, children[i]);
			}
		}
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
		out << YAML::Key << "Primary Audio Listener" << YAML::Value << scene->GetPrimaryAudioListener();
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
			SerializeEntityYAML(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		return std::string(out.c_str());
	}

	std::string Serializer::SerializeEntity(Entity entity) {
		YAML::Emitter out;
		if (!entity) return "";
		out << YAML::BeginMap;
		out << YAML::Key << "Main Entity" << YAML::Value;
		SerializeEntityYAML(out, entity, true);
		if (entity.HasComponent<ChildComponent>()) {
			std::vector<Entity> children = entity.GetComponent<ChildComponent>().Children;
			if (children.size() > 0) {
				out << YAML::Key << "Dependants" << YAML::Value << YAML::BeginSeq;
				for (int i = 0; i < children.size(); i++) {
					SerializeEntityDependancy(out, children[i]);
				}
				out << YAML::EndSeq;
			}
		}
		out << YAML::EndMap;
		return std::string(out.c_str());
	}

	std::string Serializer::SerializeRuntime() {
		// not implemented yet!
		FL_CORE_ASSERT(false);
		return "";
	}

	bool Serializer::DeserializeRawScene(Ref<Scene>& scene, const std::string& scenedata) {
		YAML::Node data = YAML::Load(scenedata);
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		FL_CORE_TRACE("Deserializing scene '{0}'", sceneName);
		scene->SetSceneName(sceneName);

		scene->SetPrimaryCamera(data["Primary Camera"].as<int>());
		scene->SetPrimaryAudioListener(data["Primary Audio Listener"].as<int>());

		scene->SetGravity(data["Physics"]["Gravity"].as<float>());
		scene->SetVelocityIterations(data["Physics"]["VelocityIterations"].as<int32_t>());
		scene->SetPositionIterations(data["Physics"]["PositionIterations"].as<int32_t>());

		auto entities = data["Entities"];
		std::map<uint32_t, uint32_t> uidmap;
		if (entities)
			for (auto entity : entities)
				DeserializeEntityYAML(entity, scene, uidmap);

		return true;
	}

	bool Serializer::DeserializeScene(Ref<Scene>& scene, const std::string& filepath) {
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();
		return DeserializeRawScene(scene, strStream.str());
	}

	Entity* Serializer::DeserializeEntity(Ref<Scene>& scene, const std::string& filepath) {
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();
		YAML::Node data = YAML::Load(strStream.str());

		if (!data["Main Entity"])
			return nullptr;

		std::map<uint32_t, uint32_t> uidmap;
		Entity mainEntity = DeserializeEntityYAML(data["Main Entity"], scene, uidmap);

		auto dependants = data["Dependants"];
		if (dependants)
			for (auto dependant : dependants)
				DeserializeEntityYAML(dependant, scene, uidmap);

		return &mainEntity;
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