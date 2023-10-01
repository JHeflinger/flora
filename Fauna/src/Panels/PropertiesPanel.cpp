#include "flpch.h"
#include "PropertiesPanel.h"
#include "Flora/Utils/PlatformUtils.h"
#include "Flora/Scripting/ScriptEngine.h"
#include "../Utils/FileUtils.h"
#include <imgui/imgui_internal.h>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>

namespace Flora {
	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uifunction, bool removeable = true) {
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>()) {
			auto& component = entity.GetComponent<T>();
			ImVec2 contentReigionAvailable = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			bool removeComponent = false;

			if (removeable) {
				ImGui::SameLine(contentReigionAvailable.x - lineHeight * 0.5f);

				if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
					ImGui::OpenPopup("ComponentSettings");

				if (ImGui::BeginPopup("ComponentSettings")) {
					if (ImGui::MenuItem("Remove component"))
						removeComponent = true;
					ImGui::EndPopup();
				}
			}

			if (open) {
				auto& src = entity.GetComponent<T>();
				ImGui::Dummy(ImVec2(0, 5.0f));
				uifunction(entity, component);
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	template<typename T>
	static void DrawAddComponentItem(std::string label, Entity entity) {
		if (!entity.HasComponent<T>()) {
			if (ImGui::MenuItem(label.c_str())) {
				entity.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		bool changed = false;

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize)) {
			values.x = resetValue;
			changed = true;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f")) changed = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 0.6f, 0.0f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize)) {
			values.y = resetValue;
			changed = true;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f")) changed = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.3f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.7f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize)) {
			values.z = resetValue;
			changed = true;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f")) changed = true;
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return changed;
	}

	static bool DrawVec2Control(const std::string& label, glm::vec2& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		bool changed = false;

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize)) {
			values.x = resetValue;
			changed = true;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f")) changed = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 0.6f, 0.0f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize)) {
			values.y = resetValue;
			changed = true;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f")) changed = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return changed;
	}

	static bool DrawVec4Control(const std::string& label, glm::vec4& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];
		bool changed = false;

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Dummy(ImVec2(0, 10.0f));
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize)) {
			values.x = resetValue;
			changed = true;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		if (ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f")) changed = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f, 0.6f, 0.0f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize)) {
			values.y = resetValue;
			changed = true;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		if (ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f")) changed = true;
		ImGui::PopItemWidth();

		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.3f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.7f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize)) {
			values.z = resetValue;
			changed = true;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		if (ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f")) changed = true;
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 1.0f, 0.9f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f, 1.0f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.9f, 0.9f, 0.0f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("W", buttonSize)) {
			values.w = resetValue;
			changed = true;
		}
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		ImGui::SameLine();
		if (ImGui::DragFloat("##W", &values.w, 0.1f, 0.0f, 0.0f, "%.2f")) changed = true;
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Dummy({ 0, 2 });
		ImGui::Columns(1);
		ImGui::PopID();

		return changed;
	}

	static void DrawTextureDropbox(const std::string& label, SpriteRendererComponent& component, ImVec2 buttonSize) {
		if (ImGui::Button(label.c_str(), buttonSize)) {
			std::string filepath = FileDialogs::OpenFile("Texture Asset (*.png)\0*.png\0");
			if (!filepath.empty()) {
				std::filesystem::path texturePath = std::filesystem::path(filepath); // warning this is not relative
				component.Filename = texturePath.filename().string();
				component.Path = texturePath.string();
				component.TextureInitialized = false;
			}
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path texturePath = std::filesystem::path("$") / path;
				if (texturePath.extension().string() == ".png") {
					component.Filename = texturePath.filename().string();
					component.Path = texturePath.string();
					component.TextureInitialized = false;
				} else {
					FL_CORE_ERROR("Invalid texture type. Please use a .png file");
				}
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::SameLine();
		ImGui::Text(component.Filename.c_str());
	}

	static void DrawAudioDropbox(const std::string& label, AudioSourceComponent& component, ImVec2 buttonSize) {
		if (ImGui::Button(label.c_str(), buttonSize)) {
			std::string filepath = FileDialogs::OpenFile("Audio Asset (*.wav)\0*.wav\0");
			if (!filepath.empty()) {
				std::filesystem::path texturePath = std::filesystem::path(filepath);
				component.AudioFile = texturePath.string();
			}
		}
		if (ImGui::BeginDragDropTarget()) {
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
				const wchar_t* path = (const wchar_t*)payload->Data;
				std::filesystem::path texturePath = Project::GetAssetDirectory() / path;
				if (texturePath.extension().string() == ".wav") {
					component.AudioFile = texturePath.string();
				}
				else {
					FL_CORE_ERROR("Invalid audio type. Please use a .wav file");
				}
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::SameLine();
		ImGui::Text(std::filesystem::path(component.AudioFile).filename().string().c_str());
	}

	static void PlayAudioFromEntity(Entity entity) {
		AudioSourceComponent asc = entity.GetComponent<AudioSourceComponent>();
		TransformComponent tfc = entity.GetComponent<TransformComponent>();
		AssetManager::AddAudio(asc.AudioFile);
		Ref<Audio> audio = AssetManager::GetAudio(asc.AudioFile);
		AudioCommand::Play(
			*audio,
			asc.Scale,
			asc.Pitch,
			asc.Gain,
			asc.Velocity,
			asc.Loop,
			tfc.Translation
		);
	}

	static void UpdateAudioFromEntity(Entity entity) {
		AudioSourceComponent asc = entity.GetComponent<AudioSourceComponent>();
		TransformComponent tfc = entity.GetComponent<TransformComponent>();
		AssetManager::AddAudio(asc.AudioFile);
		Ref<Audio> audio = AssetManager::GetAudio(asc.AudioFile);
		AudioCommand::Update(
			*audio,
			asc.Scale,
			asc.Pitch,
			asc.Gain,
			asc.Velocity,
			asc.Loop,
			tfc.Translation
		);
	}

	bool CanParseToUint32(const std::string& str) {
		if (str.empty())
			return false;

		for (char c : str)
			if (!std::isdigit(c))
				return false;

		if (str.length() > 1 && str[0] == '0')
			return false;

		try {
			size_t pos;
			uint32_t value = std::stoul(str, &pos);

			if (pos == str.length())
				return true;
		}
		catch (const std::out_of_range& e) {}

		return false;
	}

	uint32_t ParseUint32(const std::string& str) {
		try {
			size_t pos;
			uint32_t value = std::stoul(str, &pos);

			if (pos == str.length())
				return value;
			else
				FL_CORE_ASSERT(false, "Invalid 32-bit integer");
		} catch (const std::invalid_argument& e) {
			FL_CORE_ASSERT(false, "Invalid 32-bit integer");
		} catch (const std::out_of_range& e) {
			FL_CORE_ASSERT(false, "Invalid 32-bit integer");
		}
	}

	void PropertiesPanel::OnImGuiRender() {
		ImGui::Begin("Properties", &m_Enabled);
		if (ImGui::IsWindowHovered()) m_EditorContext->HoveredPanel = Panels::PROPERTIES;
		if (ImGui::IsWindowFocused()) m_EditorContext->FocusedPanel = Panels::PROPERTIES;
		Entity selectedEntity = m_EditorContext->SelectedEntity;
		if (selectedEntity)
			DrawComponents(selectedEntity);
		ImGui::End();
	}

	void PropertiesPanel::DrawComponents(Entity entity) {
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
				tag = std::string(buffer);
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("Add Component");
		if (ImGui::BeginPopup("Add Component")) {
			DrawAddComponentItem<CameraComponent>("Camera", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<SpriteRendererComponent>("Sprite Renderer", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<CircleRendererComponent>("Circle Renderer", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<ScriptComponent>("Script", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<ScriptManagerComponent>("Script Manager", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<RigidBody2DComponent>("Rigid Body 2D", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<BoxCollider2DComponent>("Box Collider 2D", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<CircleCollider2DComponent>("Circle Collider 2D", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<AudioSourceComponent>("Audio Source", m_EditorContext->SelectedEntity);
			DrawAddComponentItem<AudioListenerComponent>("Audio Listener", m_EditorContext->SelectedEntity);
			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();

		ImGuiStyle& style = ImGui::GetStyle();
		style.ItemSpacing = ImVec2(8.0f, 4.0f); // Add spacing between items in the column
		style.ColumnsMinSpacing = 20.0f;

		DrawComponent<ParentComponent>("Parent", entity, [](auto& entity, auto& component) {
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100.0f);
			ImGui::Text("Inherit All");
			ImGui::NextColumn();
			ImGui::Checkbox("##InheritAll", &component.InheritAll);
			ImGui::Columns(1);

			ImGui::Dummy(ImVec2(0, 10.0f));
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
			bool open = ImGui::TreeNodeEx((void*)typeid(ParentComponent).hash_code(), treeNodeFlags, "Advanced Settings");
			if (open) {
				if (component.InheritAll)
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.25f);
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150.0f);
				ImGui::Dummy(ImVec2(0, 8.0f));
				ImGui::Text("Inherit Transform");
				ImGui::Dummy(ImVec2(0, 8.0f));
				ImGui::Text("Inherit Sprite Properties");
				ImGui::NextColumn();
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Checkbox("##InheritTransform", &component.InheritTransform);
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Checkbox("##InheritSpriteProperties", &component.InheritSpriteProperties);
				ImGui::Columns(1);
				ImGui::TreePop();
				if (component.InheritAll)
					ImGui::PopStyleVar();
			}
		}, false);

		DrawComponent<TransformComponent>("Transform", entity, [](auto& entity, auto& component) {
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale);
		}, false);

		DrawComponent<CameraComponent>("Camera", entity, [](auto& entity, auto& component) {
			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)component.Camera.GetProjectionType()];
			float linespacing = 2.0f;
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 150.0f);
			ImGui::Text("Projection Type");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Primary Camera");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Visible Borders");
			ImGui::NextColumn();
			if (ImGui::BeginCombo("##Projection", currentProjectionTypeString)) {
				for (int i = 0; i < 2; i++) {
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
						currentProjectionTypeString = projectionTypeStrings[i];
						component.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			int primaryCameraEntity = entity.GetScene()->GetPrimaryCamera();
			bool isPrimary = false;
			if (primaryCameraEntity >= 0)
				isPrimary = (uint32_t)(primaryCameraEntity) == (uint32_t)entity;
			if (ImGui::Checkbox("##Primary", &isPrimary)) {
				if (isPrimary) {
					entity.GetScene()->SetPrimaryCamera((int)(uint32_t)entity);
				}
				else {
					entity.GetScene()->SetPrimaryCamera(-1);
				}
			}

			ImGui::Checkbox("##visible", &component.ShowBorder);
			ImGui::Columns(1);

			ImGui::Dummy(ImVec2(0, 10));
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0, 10));

			if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150.0f);
				ImGui::Text("Vertical FOV");
				ImGui::Dummy(ImVec2(0, linespacing));
				ImGui::Text("Near Clip");
				ImGui::Dummy(ImVec2(0, linespacing));
				ImGui::Text("Far Clip");
				ImGui::NextColumn();
				float perspectiveFOV = glm::degrees(component.Camera.GetPerspectiveVerticalFOV());
				if (ImGui::DragFloat("##VerticalFOV", &perspectiveFOV))
					component.Camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveFOV));
				float perspectiveNear = component.Camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("##Near", &perspectiveNear))
					component.Camera.SetPerspectiveNearClip(perspectiveNear);
				float perspectiveFar = component.Camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("##Far", &perspectiveFar))
					component.Camera.SetPerspectiveFarClip(perspectiveFar);
				ImGui::Columns(1);
			}

			if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 150.0f);
				ImGui::Text("Orthographic Size");
				ImGui::Dummy(ImVec2(0, linespacing));
				ImGui::Text("Near Clip");
				ImGui::Dummy(ImVec2(0, linespacing));
				ImGui::Text("Far Clip");
				ImGui::Dummy(ImVec2(0, linespacing));
				ImGui::Text("Fixed Aspect Ratio");
				ImGui::NextColumn();
				float orthoSize = component.Camera.GetOrthographicSize();
				if (ImGui::DragFloat("##Size", &orthoSize))
					component.Camera.SetOrthographicSize(orthoSize);
				float orthoNear = component.Camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("##Near", &orthoNear))
					component.Camera.SetOrthographicNearClip(orthoNear);
				float orthoFar = component.Camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("##Far", &orthoFar))
					component.Camera.SetOrthographicFarClip(orthoFar);
				ImGui::Checkbox("##FixedAspectRatio", &component.FixedAspectRatio);
				ImGui::Columns(1);
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite", entity, [](auto& entity, auto& component) {
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];
			const char* spriteTypeStrings[] = { "Single Texture", "Subtexture", "Animation" };
			const char* currentSpriteTypeString = spriteTypeStrings[(int)component.Type];
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 smallButtonSize = { lineHeight + 3.0f, lineHeight };
			ImVec2 largeButtonSize = { 100, lineHeight };
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
			ImGuiStyle& style = ImGui::GetStyle();
			style.ItemSpacing = ImVec2(8.0f, 4.0f); // Add spacing between items in the column
			style.ColumnsMinSpacing = 20.0f;

			//==============================================Sprite type

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100.0f);
			ImGui::Text("Type");
			ImGui::Dummy(ImVec2{ 0, 2 });
			ImGui::Text("Visible");
			ImGui::NextColumn();
			if (ImGui::BeginCombo("##Type", currentSpriteTypeString)) {
				for (int i = 0; i < 3; i++) {
					bool isSelected = currentSpriteTypeString == spriteTypeStrings[i];
					if (ImGui::Selectable(spriteTypeStrings[i], isSelected)) {
						currentSpriteTypeString = spriteTypeStrings[i];
						component.Type = (SpriteRendererComponent::SpriteType)i;
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::Checkbox("##visible", &component.Visible);
			ImGui::Columns(1);

			ImGui::Dummy(ImVec2(0, 10.0f));
			ImGui::Separator(); //===========================separator
			ImGui::Dummy(ImVec2(0, 10.0f));

			if (component.Type == SpriteRendererComponent::SpriteType::SINGLE) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100.0f);
				ImGui::Text("Texture");
				ImGui::NextColumn();
				DrawTextureDropbox("Texture", component, largeButtonSize);
				ImGui::Columns(1);

				ImGui::Dummy(ImVec2(0, 10.0f));
				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
				bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Advanced Settings");
				if (open) {
					ImGui::Dummy(ImVec2(0, 5.0f));
					ImGui::Columns(2);
					ImGui::SetColumnWidth(0, 100.0f);
					ImGui::Dummy(ImVec2(0, 8.0f));
					ImGui::Text("Tint");
					ImGui::Dummy(ImVec2(0, 10.0f));
					ImGui::Text("Tiling Factor");
					ImGui::NextColumn();
					ImGui::Dummy(ImVec2(0, 5.0f));
					ImGui::ColorEdit4("##Tint", glm::value_ptr(component.Color));
					ImGui::Dummy(ImVec2(0, 5.0f));
					ImGui::DragFloat("##Tiling Factor", &component.TilingFactor, 0.1, 0.0f, 100.0f, "%.2f");
					ImGui::Columns(1);
					ImGui::TreePop();
				}
			}

			if (component.Type == SpriteRendererComponent::SpriteType::SUBTEXTURE) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100.0f);
				ImGui::Text("Texture");
				ImGui::Dummy(ImVec2(0, 15.0f));
				ImGui::Text("Rows");
				ImGui::Dummy(ImVec2(0, 2.0f));
				ImGui::Text("Columns");
				ImGui::Dummy(ImVec2(0, 2.0f));
				ImGui::Text("Coordinate");
				ImGui::NextColumn();
				ImGui::PushItemWidth(60 + smallButtonSize.x * 2);
				DrawTextureDropbox("Subtexture", component, largeButtonSize);
				ImGui::Dummy(ImVec2(0, 5.0f));
				// rows, columns, subtexture row/col
				ImGui::DragInt("##Rows", &component.Rows, 0.1, 1, 1000);
				ImGui::DragInt("##Columns", &component.Columns, 0.1, 1, 1000);

				ImGui::PushItemWidth(30);
				style.ItemSpacing = ImVec2(0, 0); // remove spacing

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.3f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
				ImGui::PushFont(boldFont);
				if (ImGui::Button("R", smallButtonSize))
					component.RowCoordinate = 0;
				ImGui::PopStyleColor(3);
				ImGui::PopFont();

				ImGui::SameLine();
				ImGui::DragInt("##R", &component.RowCoordinate, 0.1f, 1, 10000);
				ImGui::SameLine();

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.3f, 0.8f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.0f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.7f, 1.0f });
				ImGui::PushFont(boldFont);
				if (ImGui::Button("C", smallButtonSize))
					component.ColumnCoordinate = 0;
				ImGui::PopStyleColor(3);
				ImGui::PopFont();

				ImGui::SameLine();
				ImGui::DragInt("##C", &component.ColumnCoordinate, 0.1f, 1, 10000);

				ImGui::PopItemWidth();
				style.ItemSpacing = ImVec2(8.0f, 4.0f); // Add back spacing

				ImGui::PopItemWidth();
				ImGui::Columns(1);

				ImGui::Dummy(ImVec2(0, 10.0f));
				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
				bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Advanced Settings");
				if (open) {
					// subtexture width/height
					float linespacing = 2.0f;
					ImGui::Columns(2);
					ImGui::SetColumnWidth(0, 100.0f);
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Width (Cells)");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Height (Cells)");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Tint");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Tiling Factor");
					ImGui::NextColumn();
					ImGui::DragFloat("##Width", &component.SubtextureWidth, 0.1, 0, 10000.0f);
					ImGui::DragFloat("##Height", &component.SubtextureHeight, 0.1, 0, 10000.0f);
					ImGui::ColorEdit4("##Tint", glm::value_ptr(component.Color));
					ImGui::DragFloat("##Tiling Factor", &component.TilingFactor, 0.1, 0.0f, 100.0f, "%.2f");
					ImGui::Columns(1);
					ImGui::TreePop();
				}
			}

			if (component.Type == SpriteRendererComponent::SpriteType::ANIMATION) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100.0f);
				ImGui::Text("Texture");
				ImGui::Dummy(ImVec2(0, 15.0f));
				ImGui::Text("Paused");
				ImGui::Dummy(ImVec2(0, 2.0f));
				ImGui::Text("Rows");
				ImGui::Dummy(ImVec2(0, 2.0f));
				ImGui::Text("Columns");
				ImGui::NextColumn();
				ImGui::PushItemWidth(60 + smallButtonSize.x * 2);
				DrawTextureDropbox("Sprite Sheet", component, largeButtonSize);
				ImGui::Dummy(ImVec2(0, 5.0f));
				// rows, columns, subtexture row/col
				ImGui::Checkbox("##Paused", &component.Paused);
				ImGui::DragInt("##Rows", &component.Rows, 0.1, 1, 1000);
				ImGui::DragInt("##Columns", &component.Columns, 0.1, 1, 1000);
				ImGui::PopItemWidth();
				ImGui::Columns(1);

				ImGui::Dummy(ImVec2(0, 10.0f));
				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
				bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Advanced Settings");
				if (open) {
					// subtexture width/height
					float linespacing = 2.0f;
					ImGui::Columns(2);
					ImGui::SetColumnWidth(0, 100.0f);
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Frames");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Start Frame");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("End Frame");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("FPS");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Tint");
					ImGui::Dummy(ImVec2(0, linespacing));
					ImGui::Text("Tiling Factor");
					ImGui::NextColumn();
					ImGui::DragInt("##Frames", &component.Frames, 0.1, 1, 1 + (component.Rows * component.Columns));
					ImGui::DragInt("##StartFrame", &component.StartFrame, 0.1, 1, 1 + component.Frames);
					ImGui::DragInt("##EndFrame", &component.EndFrame, 0.1, 1, 1 + component.Frames);
					ImGui::DragInt("##FPS", &component.FPS, 0.1, 1, 60);
					ImGui::ColorEdit4("##Tint", glm::value_ptr(component.Color));
					ImGui::DragFloat("##TilingFactor", &component.TilingFactor, 0.1, 0.0f, 10000.0f, "%.2f");
					ImGui::Columns(1);
					ImGui::TreePop();
				}
			}

			ImGui::PopStyleVar();
		});

		DrawComponent<CircleRendererComponent>("Circle", entity, [](auto& entity, auto& component) {
			float linespacing = 2.0f;
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100.0f);
			ImGui::Text("Thickness");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Fade");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Color");
			ImGui::NextColumn();
			ImGui::DragFloat("##Thickness", &component.Thickness, 0.01f, 0.0f, 1000000.0f, "%.3f");
			ImGui::DragFloat("##Fade", &component.Fade, 0.001f, 0.0f, 1.0f, "%.4f");
			ImGui::ColorEdit4("##Color", glm::value_ptr(component.Color));
			ImGui::Columns(1);
		});

		DrawComponent<ScriptComponent>("Script", entity, [this](auto& entity, auto& component) {
			float linespacing = 1.0f;
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 largeButtonSize = { 100, lineHeight };

			//get script button
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100.0f);
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Class");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::NextColumn();
			static bool promptSelect = false;
			std::string scriptButtonLabel = ScriptEngine::EntityClassExists(component.ClassName) ? component.ClassName : "Browse...";
			ImGuiStyle& style = ImGui::GetStyle();
			style.FramePadding.x += 10.0f;
			if (ImGui::Button(scriptButtonLabel.c_str())) promptSelect = true;
			style.FramePadding.x -= 10.0f;
			ImGui::Columns(1);

			// Select script prompt
			if (promptSelect) {
				ImGui::OpenPopup("Select Script Class");
				ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
				if (ImGui::BeginPopupModal("Select Script Class", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
					auto& classMap = ScriptEngine::GetEntityClasses();
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f, 1.0f, 1.0f, 0.15f });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 1.0f, 1.0f, 1.0f, 0.05f });
					for (auto& entityClass : classMap) {
						if (ImGui::Button(entityClass.first.c_str(), { 500, lineHeight })) {
							promptSelect = false;
							component.ClassName = entityClass.first;
						}
					}
					if (ImGui::Button("Cancel", { 500, lineHeight }))
						promptSelect = false;
					ImGui::PopStyleColor(3);
					ImGui::EndPopup();
				}
			}

			ImGui::Dummy(ImVec2(0, 10.0f));
			ImGui::Separator(); //===========================separator
			ImGui::Dummy(ImVec2(0, 10.0f));

			// Fields
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
			bool open = ImGui::TreeNodeEx((void*)typeid(ScriptComponent).hash_code(), treeNodeFlags, "Public Fields");
			if (open) {
				Ref<ScriptInstance> scriptInstance = ScriptEngine::GetEntityScriptInstance(entity);
				Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(component.ClassName);
				const auto& fields = scriptInstance ? scriptInstance->GetScriptClass()->GetFields() : entityClass->GetFields();
				auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);

				if (!scriptInstance && !entityClass) {
					ImGui::TreePop();
					return;
				}

				for (const auto& [name, field] : fields) {
					bool scriptFieldExists = entityFields.find(name) != entityFields.end();
					float data_float = 0.0f;
					glm::vec2 data_vec2 = { 0.0f, 0.0f };
					glm::vec3 data_vec3 = { 0.0f, 0.0f, 0.0f };
					glm::vec4 data_vec4 = { 0.0f, 0.0f, 0.0f, 0.0f };
					int data_int = 0;
					uint32_t data_uint = 0;
					bool data_bool = false;
					double data_double = 0.0;
					uint16_t data_short = 0;
					uint8_t data_byte = 0;
					int64_t data_entity = -1;
					std::string data_string = "";
					bool isvalid = false;
					int expanded_int = 0;
					static bool promptEntitySelect = false;
					bool scenePlaying = m_EditorContext->SceneState == SceneState::PLAY;

                    #define SET_DATA(Datavar, Type, FieldType) case ScriptFieldType::FieldType:\
																	if (scenePlaying)\
																		Datavar = scriptInstance->GetFieldValue<Type>(name);\
																	else if (scriptFieldExists) {\
																		ScriptFieldInstance& scriptField = entityFields.at(name);\
																		Datavar = scriptField.GetValue<Type>(); }\
																	break;

					switch (field.Type) {
						SET_DATA(data_float, float, Float);
						SET_DATA(data_vec2, glm::vec2, Vector2);
						SET_DATA(data_vec3, glm::vec3, Vector3);
						SET_DATA(data_vec4, glm::vec4, Vector4);
						SET_DATA(data_int, int, Int);
						// note: don't do for uint32_t, thats manually done later!
						SET_DATA(data_bool, bool, Bool);
						SET_DATA(data_double, double, Double);
						SET_DATA(data_short, uint16_t, Short);
						SET_DATA(data_byte, uint8_t, Byte);
						SET_DATA(data_entity, int64_t, Entity);
					}

					#undef SET_DATA

					#define UPDATE_DATA(Datavar) if (scenePlaying) {\
													scriptInstance->SetFieldValue(name, Datavar);\
												} else if (scriptFieldExists) {\
													ScriptFieldInstance& scriptField = entityFields.at(name);\
													scriptField.SetValue(Datavar);\
												} else {\
													ScriptFieldInstance& scriptField = entityFields[name];\
													scriptField.Field = field;\
													scriptField.SetValue(Datavar);\
												}

					bool itemShown = true;
					switch (field.Type) {
					case ScriptFieldType::Float:
						ImGui::Columns(2);
						ImGui::SetColumnWidth(0, 100.0f);
						ImGui::Text(name.c_str());
						ImGui::NextColumn();
						if (ImGui::DragFloat(("##" + name).c_str(), &data_float))
							UPDATE_DATA(data_float);
						ImGui::Columns(1);
						break;
					case ScriptFieldType::Vector2:
						if (DrawVec2Control(name, data_vec2))
							UPDATE_DATA(data_vec2);
						ImGui::Dummy(ImVec2(0, linespacing));
						break;
					case ScriptFieldType::Vector3:
						if (DrawVec3Control(name, data_vec3))
							UPDATE_DATA(data_vec3);
						ImGui::Dummy(ImVec2(0, linespacing));
						break;
					case ScriptFieldType::Vector4:
						if (DrawVec4Control(name, data_vec4))
							UPDATE_DATA(data_vec4);
						ImGui::Dummy(ImVec2(0, linespacing));
						break;
					case ScriptFieldType::Int:
						ImGui::Columns(2);
						ImGui::SetColumnWidth(0, 100.0f);
						ImGui::Dummy(ImVec2(0, linespacing));
						ImGui::Text(name.c_str());
						ImGui::Dummy(ImVec2(0, linespacing));
						ImGui::NextColumn();
						if (ImGui::DragInt(("##" + name).c_str(), &data_int)) {
							UPDATE_DATA(data_int);
						}
						ImGui::Columns(1);
						break;
					case ScriptFieldType::UInt:
						ImGui::Columns(2);
						ImGui::SetColumnWidth(0, 100.0f);
						ImGui::Dummy(ImVec2(0, linespacing));
						ImGui::Text(name.c_str());
						ImGui::Dummy(ImVec2(0, linespacing));
						ImGui::NextColumn();

						static char uint_string[2048];
						if (uint_string[0] == '\0') {
							uint_string[1] = '\0';
							uint_string[0] = '0';
						} else {
							if (scenePlaying)
								strcpy(uint_string, std::to_string(scriptInstance->GetFieldValue<uint32_t>(name)).c_str());
							else if (scriptFieldExists) {
								ScriptFieldInstance& scriptField = entityFields.at(name);
								strcpy(uint_string, std::to_string(scriptField.GetValue<uint32_t>()).c_str());
							}
						}
						isvalid = CanParseToUint32(uint_string);
						if (!isvalid)
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));
						if (ImGui::InputText(("##" + name).c_str(), uint_string, sizeof(uint_string)) && isvalid) {
							UPDATE_DATA(ParseUint32(uint_string));
						}
						if (!isvalid)
							ImGui::PopStyleColor();
						ImGui::Columns(1);
						break;
					case ScriptFieldType::Bool:
						ImGui::Columns(2);
						ImGui::SetColumnWidth(0, 100.0f);
						ImGui::Dummy(ImVec2(0, linespacing));
						ImGui::Text(name.c_str());
						ImGui::Dummy(ImVec2(0, linespacing));
						ImGui::NextColumn();
						if ((ImGui::Checkbox(("##" + name).c_str(), &data_bool))) {
							UPDATE_DATA(data_bool);
						}
						ImGui::Columns(1);
						break;
					case ScriptFieldType::Double:
						ImGui::Columns(2);
						ImGui::SetColumnWidth(0, 100.0f);
						ImGui::Dummy(ImVec2(0, linespacing));
						ImGui::Text(name.c_str());
						ImGui::Dummy(ImVec2(0, linespacing));
						ImGui::NextColumn();
						if (ImGui::InputDouble(("##" + name).c_str(), &data_double)) {
							UPDATE_DATA(data_double);
						}
						ImGui::Columns(1);
						break;
					case ScriptFieldType::Short:
						ImGui::Columns(2);
						ImGui::SetColumnWidth(0, 100.0f);
						ImGui::Dummy(ImVec2(0, linespacing));
						ImGui::Text(name.c_str());
						ImGui::Dummy(ImVec2(0, linespacing));
						ImGui::NextColumn();
						expanded_int = (int)data_short;
						if (ImGui::DragInt(("##" + name).c_str(), &expanded_int, 1.0f, -32768, 32768)) {
							data_short = (uint16_t)expanded_int;
							UPDATE_DATA(data_short);
						}
						ImGui::Columns(1);
						break;
					case ScriptFieldType::Byte:
						ImGui::Columns(2);
						ImGui::SetColumnWidth(0, 100.0f);
						ImGui::Dummy(ImVec2(0, linespacing));
						ImGui::Text(name.c_str());
						ImGui::Dummy(ImVec2(0, linespacing));
						ImGui::NextColumn();
						expanded_int = (int)data_byte;
						if (ImGui::DragInt(("##" + name).c_str(), &expanded_int, 1.0f, 0, 255)) {
							data_byte = (uint8_t)expanded_int;
							UPDATE_DATA(data_byte);
						}
						ImGui::Columns(1);
						break;
					case ScriptFieldType::Entity:
					{
						ImGui::Columns(2);
						ImGui::SetColumnWidth(0, 100.0f);
						ImGui::Dummy(ImVec2(0, linespacing));
						ImGui::Text(name.c_str());
						ImGui::Dummy(ImVec2(0, linespacing));
						ImGui::NextColumn();
						ImGuiStyle& style = ImGui::GetStyle();
						style.FramePadding.x += 10.0f;
						if (ImGui::Button("Browse..."))
							promptEntitySelect = true;
						style.FramePadding.x -= 10.0f;
						ImGui::Columns(1);

						// entity selection prompt

						if (promptEntitySelect) {
							ImGui::OpenPopup("Select Entity");
							ImVec2 center = ImGui::GetMainViewport()->GetCenter();
							ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
							if (ImGui::BeginPopupModal("Select Entity", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
								auto& entities = entity.GetScene()->GetAllEntitiesWith<TagComponent>();
								ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
								ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f, 1.0f, 1.0f, 0.15f });
								ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 1.0f, 1.0f, 1.0f, 0.05f });
								for (auto entity : entities) {
									auto tagComponent = entities.get<TagComponent>(entity);
									if (ImGui::Button(tagComponent.Tag.c_str(), { 500, lineHeight })) {
										promptEntitySelect = false;
										//TODO: need to implement this still lol
										FL_CORE_ERROR("This feature is not implemented yet");
									}
								}
								ImGui::PopStyleColor(3);
								ImGui::EndPopup();
							}
						}
						break;
					}
					default:
						itemShown = false;
						break;
					}
					if (itemShown) ImGui::Dummy(ImVec2(0, linespacing));

					#undef UPDATE_DATA
				}
				ImGui::TreePop();
			}
		});

		DrawComponent<ScriptManagerComponent>("Script Manager", entity, [](auto& entity, auto& component) {
			//TODO
			ImGui::Text("TODO");
		});

		DrawComponent<RigidBody2DComponent>("Rigid Body 2D", entity, [](auto& entity, auto& component) {
			const char* bodyTypeString[] = { "Static", "Kinematic", "Dynamic"};
			const char* currentBodyTypeString = bodyTypeString[(int)component.Type];
			float linespacing = 2.0f;
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 150.0f);
			ImGui::Text("Body Type");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Fixed Rotation");
			ImGui::NextColumn();
			if (ImGui::BeginCombo("##bodytype", currentBodyTypeString)) {
				for (int i = 0; i < 3; i++) {
					bool isSelected = currentBodyTypeString == bodyTypeString[i];
					if (ImGui::Selectable(bodyTypeString[i], isSelected)) {
						currentBodyTypeString = bodyTypeString[i];
						component.Type = (RigidBody2DComponent::BodyType)i;
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Checkbox("##fixedrotation", &component.FixedRotation);
			ImGui::Columns(1);
		});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& entity, auto& component) {
			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			auto boldFont = io.Fonts->Fonts[0];
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 smallButtonSize = { lineHeight + 3.0f, lineHeight };
			float linespacing = 2.0f;
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 150.0f);
			ImGui::Text("Offset");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Size");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Density");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Friction");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Restitution");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Rest. Threshold");
			ImGui::NextColumn();

			ImGui::PushItemWidth(50);
			style.ItemSpacing = ImVec2(0, 0); // remove spacing

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", smallButtonSize))
				component.Offset.x = 0;
			ImGui::PopStyleColor(3);
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::DragFloat("##Xoffset", &component.Offset.x, 0.1f, 1, 10000);
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.3f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.7f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", smallButtonSize))
				component.Offset.y = 0;
			ImGui::PopStyleColor(3);
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::DragFloat("##Yoffset", &component.Offset.y, 0.1f, 1, 10000);
			ImGui::Dummy(ImVec2(0, linespacing));

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("W", smallButtonSize))
				component.Size.x = 0;
			ImGui::PopStyleColor(3);
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::DragFloat("##Woffset", &component.Size.x, 0.1f, 1, 10000);
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.3f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.7f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("H", smallButtonSize))
				component.Size.y = 0;
			ImGui::PopStyleColor(3);
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::DragFloat("##Hoffset", &component.Size.y, 0.1f, 1, 10000);
			ImGui::Dummy(ImVec2(0, linespacing));

			ImGui::PopItemWidth();
			style.ItemSpacing = ImVec2(8.0f, 4.0f); // Add back spacing

			ImGui::PushItemWidth(154);
			ImGui::DragFloat("##Density", &component.Density, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::DragFloat("##Friction", &component.Friction, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::DragFloat("##Restitution", &component.Restitution, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::DragFloat("##RestitutionThreshold", &component.RestitutionThreshold, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::Columns(1);
		});

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& entity, auto& component) {
			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			auto boldFont = io.Fonts->Fonts[0];
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 smallButtonSize = { lineHeight + 3.0f, lineHeight };
			float linespacing = 2.0f;
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 150.0f);
			ImGui::Text("Offset");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Radius");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Density");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Friction");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Restitution");
			ImGui::Dummy(ImVec2(0, linespacing));
			ImGui::Text("Rest. Threshold");
			ImGui::NextColumn();

			ImGui::PushItemWidth(50);
			style.ItemSpacing = ImVec2(0, 0); // remove spacing

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.7f, 0.1f, 0.1f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", smallButtonSize))
				component.Offset.x = 0;
			ImGui::PopStyleColor(3);
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::DragFloat("##Xoffset", &component.Offset.x, 0.1f, 1, 10000);
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.3f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.2f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.7f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", smallButtonSize))
				component.Offset.y = 0;
			ImGui::PopStyleColor(3);
			ImGui::PopFont();

			ImGui::SameLine();
			ImGui::DragFloat("##Yoffset", &component.Offset.y, 0.1f, 1, 10000);
			ImGui::Dummy(ImVec2(0, linespacing));

			ImGui::PopItemWidth();
			style.ItemSpacing = ImVec2(8.0f, 4.0f); // Add back spacing

			ImGui::PushItemWidth(154);
			ImGui::DragFloat("##Radius", &component.Radius, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::DragFloat("##Density", &component.Density, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::DragFloat("##Friction", &component.Friction, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::DragFloat("##Restitution", &component.Restitution, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::DragFloat("##RestitutionThreshold", &component.RestitutionThreshold, 0.1, 0.0f, 10000.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::Columns(1);
		});
	
		DrawComponent<AudioSourceComponent>("Audio Source", entity, [](auto& entity, auto& component) {
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 150.0f);
			ImGui::Text("Dev Control");
			ImGui::Dummy({0, 2});
			ImGui::Text("Audio File");
			ImGui::NextColumn();
			ImGuiStyle& style = ImGui::GetStyle();
			style.FramePadding.x += 10.0f;
			bool isValid = component.AudioFile != "";
			std::string playBtnLabel = "Play";
			bool isPlaying = false;
			if (isValid) {
				AssetManager::AddAudio(component.AudioFile);
				Ref<Audio> audio = AssetManager::GetAudio(component.AudioFile);
				if (AudioCommand::IsPlaying(*audio)) {
					playBtnLabel = "Stop";
					isPlaying = true;
				}
			}
			
			if (ImGui::Button(playBtnLabel.c_str(), {100.0f, 0.0f})) {
				if (isValid) {
					if (!isPlaying) {
						PlayAudioFromEntity(entity);
					} else {
						AssetManager::AddAudio(component.AudioFile);
						Ref<Audio> audio = AssetManager::GetAudio(component.AudioFile);
						AudioCommand::Stop(*audio);
					}
				}
			}
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			DrawAudioDropbox("Browse...", component, { 100, lineHeight });
			style.FramePadding.x -= 10.0f;
			ImGui::Columns(1);

			ImGui::Dummy(ImVec2(0, 10.0f));
			ImGui::Separator(); //===========================separator
			ImGui::Dummy(ImVec2(0, 10.0f));

			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
			bool open = ImGui::TreeNodeEx((void*)typeid(ScriptComponent).hash_code(), treeNodeFlags, "Advanced Options");
			if (open) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100.0f);
				ImGui::Text("Scale");
				ImGui::Dummy({ 0, 2 });
				ImGui::Text("Loop");
				ImGui::Dummy({ 0, 2 });
				ImGui::Text("Pitch");
				ImGui::Dummy({ 0, 2 });
				ImGui::Text("Gain");
				ImGui::NextColumn();

				ImGui::DragFloat("##scale", &component.Scale);
				ImGui::Checkbox("##loop", &component.Loop);
				ImGui::DragFloat("##pitch", &component.Pitch);
				ImGui::DragFloat("##gain", &component.Gain, 0.01f, 0.0f, 1.0f);
				ImGui::Columns(1);
				ImGui::Dummy({ 0, 2 });

				DrawVec3Control("Velocity", component.Velocity);
				ImGui::TreePop();
			}
		});
	
		DrawComponent<AudioListenerComponent>("Audio Listener", entity, [](auto& entity, auto& component) {
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 150.0f);
			ImGui::Text("Primary Listener");
			ImGui::NextColumn();
			int primaryAudioListener = entity.GetScene()->GetPrimaryAudioListener();
			bool isPrimary = false;
			if (primaryAudioListener >= 0)
				isPrimary = (uint32_t)(primaryAudioListener) == (uint32_t)entity;
			if (ImGui::Checkbox("##Primary", &isPrimary)) {
				if (isPrimary) {
					entity.GetScene()->SetPrimaryAudioListener((int)(uint32_t)entity);
				} else {
					entity.GetScene()->SetPrimaryAudioListener(-1);
				}
			}
			ImGui::Columns(1);
			ImGui::Dummy(ImVec2(0, 10.0f));
			ImGui::Separator(); //===========================separator
			ImGui::Dummy(ImVec2(0, 10.0f));

			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
			bool open = ImGui::TreeNodeEx((void*)typeid(ScriptComponent).hash_code(), treeNodeFlags, "Advanced Options");
			if (open) {
				ImGui::Columns(2);
				ImGui::SetColumnWidth(0, 100.0f);
				ImGui::Text("Gain");
				ImGui::NextColumn();
				ImGui::DragFloat("##gain", &component.Gain, 0.01f, 0.0f, 1.0f);
				ImGui::Columns(1);
				ImGui::Dummy({ 0, 2 });

				DrawVec3Control("Velocity", component.Velocity);
				ImGui::TreePop();
			}
		});
	
		ImGui::Dummy({ 0, 10 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		if (ImGui::Button("Export Entity", { -1, lineHeight })) {
			FileUtils::ExportEntity(entity);
		}
	}
}