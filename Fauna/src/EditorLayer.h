#pragma once
#include "Flora.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/StatsPanel.h"
#include "Flora/Renderer/EditorCamera.h"
#include "Flora/Utils/Structures.h"
#include <map>

namespace Flora {
	class EditorLayer : public Layer {
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttatch() override;
		virtual void OnDetatch() override;
		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		void OnOverrideEvent();
		void ResetEditorParams();
		void InitializePanels();
		void SetPanelContext();
		void UpdatePanels();
		void RenderImGuiPanels();
		void UpdateEditorParams(Timestep ts);
		void AutoSaveEditor(Timestep ts);
	private:
		template<typename T>
		T* GetSpecificPanel(std::string key) { return dynamic_cast<T*>(m_Panels[key].get()); }
	private:
		// Panels
		std::map<std::string, Scope<EditorPanel>> m_Panels;

		// OverrideEvent - temp solution
		bool m_OverrideEventReady = true;

		// Serializer Settings
		Ref<EditorParams> m_EditorParams;
	};
}