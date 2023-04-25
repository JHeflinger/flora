#include "flpch.h"
#include "ContentBrowserPanel.h"
#include <imgui/imgui.h>
#include <stdio.h>
#include "../Utils/FileUtils.h"

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
		if (ImGui::IsWindowHovered()) m_EditorContext->HoveredPanel = Panels::CONTENTBROWSER;
		if (ImGui::IsWindowFocused()) m_EditorContext->FocusedPanel = Panels::CONTENTBROWSER;

		if (m_CurrentDirectory != std::filesystem::path(g_AssetPath)) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::ImageButton((ImTextureID)m_BackIcon->GetRendererID(), { 20, 20 }, { 0, 1 }, { 1, 0 }))
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path payloadPath = std::filesystem::path(g_AssetPath) / path;
					std::string newPath = payloadPath.parent_path().parent_path().string() + "/" + payloadPath.filename().string();
					if (std::rename(payloadPath.string().c_str(), newPath.c_str()) != 0) FL_CORE_ERROR("Error moving file");
				}
				ImGui::EndDragDropTarget();
			}

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

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) || !ImGui::IsWindowFocused())
			m_SelectedFile = "";
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_AssetPath);
			std::string filenameString = relativePath.filename().string();

			ImGui::PushID(filenameString.c_str());

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			
			if (m_SelectedFile != path.string()) {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 }))
					m_SelectedFile = path.string();
				ImGui::PopStyleColor();
			} else ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource()) {
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			if (directoryEntry.is_directory()) {
				if (ImGui::BeginDragDropTarget()) {
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path payloadPath = std::filesystem::path(g_AssetPath) / path;
						std::string newPath = payloadPath.parent_path().string() + "/" + filenameString + "/" + payloadPath.filename().string();
						if (std::rename(payloadPath.string().c_str(), newPath.c_str()) != 0) FL_CORE_ERROR("Error moving file");
					}
					ImGui::EndDragDropTarget();
				}
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
				else if (path.extension().string() == ".flora")
					RequestOpenScene(path.string());
			}

			if (ImGui::BeginPopupContextItem()) {
				if (path.extension().string() == ".flora")
					if (ImGui::MenuItem("Open")) 
						RequestOpenScene(path.string());
				if (ImGui::MenuItem("Rename"))
					m_FileToRename = path.string();
				if (ImGui::MenuItem("Delete")) 
					if (std::filesystem::is_directory(path)) {
						std::filesystem::remove_all(path.string().c_str());
					} else {
						if (std::remove(path.string().c_str()) != 0) FL_CORE_ERROR("Error deleting file");
					}
				if (ImGui::MenuItem("Cut")) {
					m_EditorContext->Clipboard.Filepath = path.string();
					m_EditorContext->Clipboard.CutFile = true;
				}
				if (ImGui::MenuItem("Copy")) {
					m_EditorContext->Clipboard.Filepath = path.string();
					m_EditorContext->Clipboard.CutFile = false;
				}
				ImGui::EndPopup();
			}

			if (path.string() == m_FileToRename) {
				static char newFilename[256] = "";
				strncpy(newFilename, path.filename().string().c_str(), 256);
				ImGui::SetNextItemWidth(thumbnailSize);
				bool enterPressed = ImGui::InputText("##Rename", newFilename, 256, ImGuiInputTextFlags_EnterReturnsTrue);
				if (enterPressed) {
					std::string completeFilename = m_CurrentDirectory.string() + "/" + std::string(newFilename);
					if (std::rename(path.string().c_str(), completeFilename.c_str()) == 0) {
						m_FileToRename = "";
					} else FL_CORE_ERROR("Error renaming file");
				}
			} else {
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + thumbnailSize / 2 - ImGui::CalcTextSize(filenameString.c_str()).x / 2);
				ImGui::TextWrapped(filenameString.c_str());
			}
			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::Columns(1);

		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems)) {
			if (ImGui::MenuItem("New Folder")) {
				std::filesystem::path newFolderPath = m_CurrentDirectory / "New Folder";
				std::filesystem::create_directory(newFolderPath);
			}
			if (ImGui::MenuItem("New Script")) {
				// TODO
				FL_CORE_ERROR("New Script has not been implemented yet!");
			}
			if (ImGui::MenuItem("New Scene")) {
				// TODO
				FL_CORE_ERROR("New Scene has not been implemented yet!");
			}
			if (ImGui::MenuItem("Paste")) {
				if (!m_EditorContext->Clipboard.CutFile) {
					std::string copyfile = m_EditorContext->Clipboard.Filepath;
					if (copyfile != "") {
						if (std::filesystem::is_directory(copyfile)) {
							std::string newFolder = m_CurrentDirectory.string() + "\\"
								+ std::filesystem::path(copyfile).filename().string();
							int i = 0;
							while (std::filesystem::exists(newFolder)) {
								i++;
								newFolder = std::filesystem::path(copyfile).parent_path().string() + "/" +
									std::filesystem::path(copyfile).stem().string() + " (" + std::to_string(i) + ")" +
									std::filesystem::path(copyfile).extension().string();
							}
							FileUtils::CopyDirectory(copyfile, newFolder);
						}
						else {
							std::string newfile = m_CurrentDirectory.string() + "\\"
								+ std::filesystem::path(copyfile).filename().string();
							int i = 0;
							while (std::filesystem::exists(newfile)) {
								i++;
								newfile = std::filesystem::path(copyfile).parent_path().string() + "/" +
									std::filesystem::path(copyfile).stem().string() + " (" + std::to_string(i) + ")" +
									std::filesystem::path(copyfile).extension().string();
							}
							std::ifstream infile(copyfile.c_str(), std::ios::binary);
							std::ofstream outfile(newfile.c_str(), std::ios::binary);
							if (infile.is_open() && outfile.is_open()) {
								outfile << infile.rdbuf();
								infile.close();
								outfile.close();
							}
							else FL_CORE_ERROR("Error pasting file");
						}
					}
				}
				else {
					std::string copyfile = m_EditorContext->Clipboard.Filepath;
					std::string newfile = m_CurrentDirectory.string() + "\\" +
						std::filesystem::path(copyfile).stem().string() +
						std::filesystem::path(copyfile).extension().string();
					if (std::rename(copyfile.c_str(), newfile.c_str()) != 0)
						FL_CORE_ERROR("Error moving file");
					else m_SelectedFile = "";
				}
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}
}