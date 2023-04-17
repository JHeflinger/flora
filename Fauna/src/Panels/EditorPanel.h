#pragma once
#include "Flora/Scene/Scene.h"
#include "Flora/Utils/Structures.h"

namespace Flora {
	class EditorPanel {
	public:
		virtual ~EditorPanel() = default;
		virtual void Initialize() = 0;
		virtual void SetEditorContext(const Ref<EditorParams>& params) { m_EditorContext = params; }
		virtual void OnUpdate() = 0;
		virtual void OnImGuiRender() = 0;
		virtual void TogglePanel() { m_Enabled = !m_Enabled; }
		virtual bool GetPanelEnabled() { return m_Enabled; }
	protected:
		bool m_Enabled = true;
		Ref<EditorParams> m_EditorContext;
	};
}