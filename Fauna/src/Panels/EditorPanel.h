#pragma once
#include "Flora/Scene/Scene.h"
#include "Flora/Utils/Structures.h"

namespace Flora {
	class EditorPanel {
	public:
		virtual ~EditorPanel() = default;
		virtual void Initialize() = 0;
		virtual void SetSceneContext(const Ref<Scene>& context) { m_SceneContext = context; }
		virtual void SetEditorContext(const Ref<EditorParams>& params) { m_EditorContext = params; }
		virtual void OnUpdate() = 0;
		virtual void OnImGuiRender() = 0;
	protected:
		Ref<Scene> m_SceneContext;
		Ref<EditorParams> m_EditorContext;
	};
}