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
	private:
		std::filesystem::path m_CurrentDirectory; 
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};
}