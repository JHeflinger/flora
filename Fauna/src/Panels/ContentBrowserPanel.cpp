#include "flpch.h"
#include "ContentBrowserPanel.h"
#include <imgui/imgui.h>

namespace Flora {
	// TODO: change this when projects are implemented
	extern const std::filesystem::path g_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetPath) {
		m_DirectoryIcon = Texture2D::Create("resources/icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("resources/icons/ContentBrowser/FileIcon.png");
		m_BackIcon = Texture2D::Create("resources/icons/ContentBrowser/BackIcon.png");
		m_UpIcon = Texture2D::Create("resources/icons/ContentBrowser/UpIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender() {
		ImGui::Begin("Content Browser", &m_Enabled);

		if (m_CurrentDirectory != std::filesystem::path(g_AssetPath)) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::ImageButton((ImTextureID)m_BackIcon->GetRendererID(), { 20, 20 }, { 0, 1 }, { 1, 0 }))
				m_CurrentDirectory = m_CurrentDirectory.parent_path();

			ImGui::SameLine(ImGui::GetWindowSize().x);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 30);
			if (ImGui::ImageButton((ImTextureID)m_UpIcon->GetRendererID(), { 20, 20 }, { 0, 1 }, { 1, 0 }))
				m_CurrentDirectory = std::filesystem::path(g_AssetPath);
			ImGui::PopStyleColor();
		}
		else {
			ImGui::Dummy({ 0, 26 });
		}

		static float padding = 16.0f;
		static float thumbnailSize = 110.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		float columnWidth = ImGui::GetWindowContentRegionWidth() / columnCount;

		int i = 0;
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {	
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_AssetPath);
			std::string filenameString = relativePath.filename().string();

			ImGui::PushID(filenameString.c_str());

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			ImGui::PopStyleColor();

			if (ImGui::BeginDragDropSource()) {
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
				else if (path.extension().string() == ".flora")
					RequestOpenScene(path.string());
			}

			ImGui::SetCursorPosX((i*columnWidth) + columnWidth / 2 - ImGui::CalcTextSize(filenameString.c_str()).x / 2);
			ImGui::TextWrapped(filenameString.c_str());
			ImGui::NextColumn();
			ImGui::PopID();
			i++;
		}

		ImGui::Columns(1);

		// TODO: status bar
		ImGui::End();
	}
}