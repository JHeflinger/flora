#pragma once
#include "Flora/Scene/Scene.h"

namespace Flora {
	class SceneSerializer {
	public:
		SceneSerializer(const Ref<Scene>& scene);
		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);
		void SerializeEditor(EditorCamera& editorCamera, const std::string& filepath = "assets/settings/fauna.fnproj");
		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);
		bool DeserializeEditor(EditorCamera& editorCamera, const std::string& filepath = "assets/settings/fauna.fnproj");
	private:
		Ref<Scene> m_Scene;
		Ref<EditorCamera> m_EditorCamera;
	};
}