#pragma once
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"
#include "Flora/Core/Layer.h"
#include "Flora/Project/Project.h"

namespace Flora {
	enum class Panels {
		NONE            = 0,
		CONSOLE         = 1,
		CONTENTBROWSER  = 2,
		PROPERTIES      = 3,
		SCENEHIERARCHY  = 4,
		STATS           = 5,
		VIEWPORT        = 6
	};

	enum class SceneState { EDIT = 0, PLAY = 1 };

	enum class Stats {
		DRAWCALLS       = 0,
		QUADS           = 1,
		CIRCLES         = 2,
		LINES           = 3,
		VERTICES        = 4,
		INDICES         = 5,
		FRAMETIME       = 6,
		FPS             = 7,
		LOWEST_FPS      = 8,
		HIGHEST_FPS     = 9,
		CPU_USAGE       = 10,
		GPU_USAGE       = 11,
		MEM_USAGE       = 12,
		DISK_USAGE      = 13
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
		bool VisibleColliders = false;
		std::vector<std::string> ClosedPanels; //more runtime-efficient way to do this would be with a hashmap
		float Time; // in seconds
		Panels FocusedPanel = Panels::NONE;
		Panels HoveredPanel = Panels::NONE;
		Clipboard Clipboard;
		SceneState SceneState = SceneState::EDIT;
		std::string Error = "";
		Ref<Project> Project;
		std::string ProjectFilepath = "";
		
		// stats panel settings
		float Timeframe = 5.0f;
		float AxisScale = 1.0f;
		float AxisMinimum = 0.0f;
		bool ShowGraphKey = true;
		std::map<Stats, bool> ShowStatMap;

		// crash detection
		bool Crashed = false;
	};
}