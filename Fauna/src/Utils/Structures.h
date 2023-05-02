#pragma once
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"
#include "Flora/Core/Layer.h"

namespace Flora {
	enum class Panels {
		NONE = 0,
		CONSOLE = 1,
		CONTENTBROWSER = 2,
		PROPERTIES = 3,
		SCENEHIERARCHY = 4,
		STATS = 5,
		VIEWPORT = 6
	};

	enum class SceneState { EDIT = 0, PLAY = 1 };

	struct Clipboard {
		std::string Filepath = "";
		Entity Entity = {};
		bool CutFile = false;
		bool CutEntity = false;
	};

	struct EditorParams {
		Entity SelectedEntity = {};
		int GizmoType = -1;
		Ref<Scene> ActiveScene;
		EditorCamera EditorCamera;
		bool Resized = true;
		std::vector<std::string> ClosedPanels; //more runtime-efficient way to do this would be with a hashmap
		float Time; // in seconds
		Panels FocusedPanel = Panels::NONE;
		Panels HoveredPanel = Panels::NONE;
		Clipboard Clipboard;
		SceneState SceneState = SceneState::EDIT;
	};
}