#pragma once

namespace Flora {
	class SceneManager {
	public:
		SceneManager();
		std::string GetScene(std::string scene);
		std::string GetStartingScene();
	private:
		std::unordered_map<std::string, std::string> m_SceneMap;
	};
}