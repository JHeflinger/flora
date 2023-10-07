#pragma once
#include "Flora.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/StatsPanel.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/ConsolePanel.h"
#include "Panels/PhysicsPanel.h"
#include "Flora/Renderer/EditorCamera.h"
#include "Utils/Structures.h"
#include "Flora/Events/KeyEvent.h"
#include <map>

namespace Flora {
	enum class SavePromptType { NONE = 0, NORM = 1, FINAL = 2, OPEN = 3, NEW = 4, OPENPATH = 5 };
	enum class ProjectPromptType { NONE = 0, NEW = 1, EDIT = 2, OPEN = 3 };
	enum class SystemPromptType { NONE = 0, CRASH = 1 };

	class EditorLayer : public Layer {
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual bool RequestCloseProtection() override { return true; }
		virtual bool ConfirmClose() override { return m_ReadyToClose; }
		virtual void ProcessWindowClose() override;
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
		void InitializeEditor();
		void InitializeSystems();
		void SetPanelContext();
		void UpdatePanels(Timestep ts);
		void RenderImGuiPanels();
		void RenderSystemPrompt();
		void UpdateEditor(Timestep ts);
		void AutoSaveEditor(Timestep ts);
		std::string GetLastSavedString();
		void PromptSave(SavePromptType type) { m_SavePromptType = type; }
		void RenderSavePrompt();
		void RenderErrorPrompt();
		void RenderProjectPrompt();
		void RenderDebugOverlay();
		void RenderUIBar();
		void OnScenePlay();
		void OnSceneStop();
		void OnScenePause();
		void OnSceneStep();
	private:
		void DevEvent();
	private:
		template<typename T>
		T* GetSpecificPanel(std::string key) { return dynamic_cast<T*>(m_Panels[key].get()); }
	private:
		// Panels
		std::map<std::string, Scope<EditorPanel>> m_Panels;

		// OverrideEvent - temp solution
		bool m_OverrideEventReady = true;

		// prompt vars
		SavePromptType m_SavePromptType = SavePromptType::NONE;
		ProjectPromptType m_ProjectPromptType = ProjectPromptType::NONE;
		SystemPromptType m_SystemPromptType = SystemPromptType::NONE;

		// process before close var
		bool m_ReadyToClose = false;

		// Serializer Settings
		Ref<EditorParams> m_EditorParams;

		// Toolbar Icons
		Ref<Texture2D> m_IconPlay;
		Ref<Texture2D> m_IconStop;
		Ref<Texture2D> m_IconPause;
		Ref<Texture2D> m_IconStep;
	};
}