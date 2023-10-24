#pragma once
#include "Flora/Scene/Scene.h"
#include "Flora/Utils/YamlUtils.h"

namespace Flora {
	class Serializer {
	public:
		static void SerializeFile(const std::string content, const std::string& filepath);
		static std::string SerializeScene(Ref<Scene>& scene);
		static std::string SerializeEntity(Entity entity);
		static std::string SerializeRuntime();
		static bool DeserializeScene(Ref<Scene>& scene, const std::string& filepath);
		static Entity* DeserializeEntity(Ref<Scene>& scene, const std::string& filepath);
		static bool DeserializeRuntime();
		static bool IsSceneSaved(Ref<Scene>& scene);
	public:
		static glm::vec3 GetNodeAsVec3(YAML::Node node);
		static glm::vec4 GetNodeAsVec4(YAML::Node node);
		static std::vector<std::string> GetNodeAsVecString(YAML::Node node);
	};
}