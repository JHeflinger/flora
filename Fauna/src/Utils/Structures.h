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

	enum class Stats {
		DRAWCALLS = 0,
		QUADS = 1,
		VERTICES = 2,
		INDICES = 3,
		FRAMETIME = 4,
		FPS = 5,
		LOWEST_FPS = 6,
		HIGHEST_FPS = 7
	};

	struct Clipboard {
		std::string Filepath = "";
		Entity Entity = {};
		bool CutFile = false;
		bool CutEntity = false;
	};

	struct EditorParams {
		// general editor settings
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
		
		// stats panel settings
		float Timeframe = 5.0f;
		bool ShowGraphKey = true;
		std::map<Stats, bool> ShowStatMap;
	};
}