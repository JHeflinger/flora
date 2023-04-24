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
	};
}