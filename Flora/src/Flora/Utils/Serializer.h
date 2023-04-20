#pragma once
#include "Flora/Scene/Scene.h"
#include "Flora/Utils/Structures.h"

namespace Flora {
	class Serializer {
	public:
		static void SerializeFile(const std::string content, const std::string& filepath);
		static std::string SerializeScene(Ref<Scene>& scene);
		static std::string SerializeRuntime();
		static std::string SerializeEditor(Ref<EditorParams> params);
		static bool DeserializeScene(Ref<Scene>& scene, const std::string& filepath);
		static bool DeserializeRuntime();
		static bool DeserializeEditor(Ref<EditorParams> params, const std::string& filepath = "assets/settings/fauna.fnproj");
		static bool IsSceneSaved(Ref<Scene>& scene);
	};
}