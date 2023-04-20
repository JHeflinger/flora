#pragma once
#include "EditorPanel.h"
#include <filesystem>
#include "Flora/Renderer/Texture.h"

namespace Flora {
	class ContentBrowserPanel : public EditorPanel {
	public:
		ContentBrowserPanel();
		virtual void Initialize() override {};
		virtual void OnUpdate() override {};
		void OnImGuiRender() override;
	public:
		void RequestOpenScene(std::string filepath) { m_OpenNewScene = true; newScenePath = filepath; }
		bool IsRequestingOpenScene() { return m_OpenNewScene; }
		void ResolveOpenSceneRequest() { m_OpenNewScene = false; }
		std::string GetRequestedStringPath() { return newScenePath; }
	private:
		std::filesystem::path m_CurrentDirectory; 
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_BackIcon;
		Ref<Texture2D> m_UpIcon;
	private:
		bool m_OpenNewScene = false;
		std::string newScenePath = "";
	};
}