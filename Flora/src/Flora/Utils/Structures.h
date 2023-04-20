#pragma once
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"
#include "Flora/Core/Layer.h"

namespace Flora {
	struct EditorParams {
		Entity SelectedEntity = {};
		int GizmoType = -1;
		Ref<Scene> ActiveScene;
		EditorCamera EditorCamera;
		bool Resized = true;
		std::vector<std::string> ClosedPanels; //more runtime-efficient way to do this would be with a hashmap
		float Time; // in seconds
	};
}