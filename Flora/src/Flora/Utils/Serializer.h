#pragma once
#include "Flora/Scene/Scene.h"
#include "Flora/Utils/Structures.h"

namespace Flora {
	class Serializer {
	public:
		static void SerializeScene(Ref<Scene>& scene, const std::string& filepath);
		static void SerializeRuntime();
		static void SerializeEditor(Ref<EditorParams> params, const std::string& filepath = "assets/settings/fauna.fnproj");
		static bool DeserializeScene(Ref<Scene>& scene, const std::string& filepath);
		static bool DeserializeRuntime();
		static bool DeserializeEditor(Ref<EditorParams> params, const std::string& filepath = "assets/settings/fauna.fnproj");
	};
}