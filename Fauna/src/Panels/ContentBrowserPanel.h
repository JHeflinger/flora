#pragma once
#include "EditorPanel.h"
#include <filesystem>
#include "Flora/Renderer/Texture.h"

namespace Flora {
	class ContentBrowserPanel : public EditorPanel {
	public:
		ContentBrowserPanel();
		virtual void Initialize();
		virtual void OnUpdate() override {};
		void OnImGuiRender() override;
	public:
		void RequestOpenScene(std::string filepath) { m_OpenNewScene = true; m_NewScenePath = filepath; }
		bool IsRequestingOpenScene() { return m_OpenNewScene; }
		void ResolveOpenSceneRequest() { m_OpenNewScene = false; }
		std::string GetRequestedStringPath() { return m_NewScenePath; }
		std::string GetSelectedFile() { return m_SelectedFile; }
		std::string GetBrowserDirectory() { return m_CurrentDirectory.string(); }
		void SetSelectedFile(std::string newfile) { m_SelectedFile = newfile; }
	private:
		std::filesystem::path m_CurrentDirectory; 
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_BackIcon;
		Ref<Texture2D> m_UpIcon;
	private:
		bool m_OpenNewScene = false;
		std::string m_SelectedFile = "";
		std::string m_FileToRename = "";
		std::string m_NewScenePath = "";
	};
}