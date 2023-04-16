#pragma once
#include "Flora/Scene/Scene.h"
#include "Flora/Scene/Entity.h"

namespace Flora {
	struct EditorParams {
		Entity SelectedEntity = {};
		int GizmoType = -1;
		Ref<Scene> ActiveScene;
		EditorCamera EditorCamera;
		bool Resized = true;
	};
}