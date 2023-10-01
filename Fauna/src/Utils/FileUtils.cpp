#include "flpch.h"
#include "FileUtils.h"
#include "Flora/Utils/PlatformUtils.h"
#include "EditorSerializer.h"
#include "Flora/Utils/Serializer.h"
#include "Flora/Project/Project.h"
#include <shellapi.h>
#include <locale>
#include <codecvt>

namespace Flora {
	static std::string ConvertWCharToString(const wchar_t* wstr) {
		int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
		if (size == 0)
			return "";

		std::string utf8Str;
		utf8Str.resize(size);
		WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &utf8Str[0], size, nullptr, nullptr);

		return utf8Str;
	}

	void FileUtils::OpenScene(Ref<EditorParams> context){
		std::string filepath = FileDialogs::OpenFile("Flora Scene (*.flora)\0*.flora\0");
		if (!filepath.empty()) {
			OpenScene(context, filepath);
		}
	}

	Entity* FileUtils::ImportEntity(Ref<EditorParams> context) {
		std::string filepath = FileDialogs::OpenFile("Flora Entity (*.flent)\0*.flent\0");
		if (!filepath.empty()) {
			return ImportEntity(context, filepath);
		}
		return nullptr;
	}

	void FileUtils::SaveSceneAs(Ref<EditorParams> context){
		std::string filepath = FileDialogs::SaveFile("Flora Scene (*.flora)\0*.flora\0");
		if (!filepath.empty()) {
			if (std::filesystem::path(filepath).stem().extension().string() != ".flora") filepath = filepath + ".flora";
			std::string sceneContent = Serializer::SerializeScene(context->ActiveScene);
			Serializer::SerializeFile(sceneContent, filepath);
			context->ActiveScene->SetSceneFilepath(filepath);
		}
	}

	void FileUtils::ExportEntity(Entity entity) {
		std::string filepath = FileDialogs::SaveFile("Flora Entity (*.flent)\0*.flent\0");
		if (!filepath.empty()) {
			if (std::filesystem::path(filepath).stem().extension().string() != ".flent") filepath = filepath + ".flent";
			std::string entityContent = Serializer::SerializeEntity(entity);
			Serializer::SerializeFile(entityContent, filepath);
		}
	}

	void FileUtils::SaveScene(Ref<EditorParams> context){
		std::string sceneFilepath = context->ActiveScene->GetSceneFilepath();
		if (sceneFilepath != "NULL") {
			std::string sceneContent = Serializer::SerializeScene(context->ActiveScene);
			Serializer::SerializeFile(sceneContent, sceneFilepath);
		}
		else {
			SaveSceneAs(context);
		}
	}

	void FileUtils::SaveTempScene(Ref<EditorParams> context) {
		std::string sceneContent = Serializer::SerializeScene(context->ActiveScene);
		Serializer::SerializeFile(sceneContent, "Resources/Temp/tempScene.flora");
		std::string editorContent = EditorSerializer::Serialize(context);
		Serializer::SerializeFile(editorContent, "Resources/Temp/tempEditorSettings.fnproj");
	}

	void FileUtils::OpenTempScene(Ref<EditorParams> context) {
		bool entitySelected = context->SelectedEntity;
		uint32_t selectedEntityID = context->SelectedEntity;
		EditorSerializer::Deserialize(context, "Resources/Temp/tempEditorSettings.fnproj");
		std::string sceneFilepath = context->ActiveScene->GetSceneFilepath();
		NewScene(context);
		context->ActiveScene->SetSceneFilepath(sceneFilepath);
		Serializer::DeserializeScene(context->ActiveScene, "Resources/Temp/tempscene.flora");
		if (entitySelected)
			context->SelectedEntity = Entity{ (entt::entity)selectedEntityID, &(*(context->ActiveScene)) };
		else
			context->SelectedEntity = Entity{};
	}

	void FileUtils::OpenScene(Ref<EditorParams> context, const std::filesystem::path& path){
		NewScene(context);
		context->ActiveScene->SetSceneFilepath(path.string());
		context->SelectedEntity = {};
		Serializer::DeserializeScene(context->ActiveScene, path.string());
		std::string editorContent = EditorSerializer::Serialize(context);
		Serializer::SerializeFile(editorContent, "Resources/Settings/fauna.fnproj");
	}

	Entity* FileUtils::ImportEntity(Ref<EditorParams> context, const std::filesystem::path& path) {
		return Serializer::DeserializeEntity(context->ActiveScene, path.string());
	}

	void FileUtils::NewScene(Ref<EditorParams> context){
		context->ActiveScene = CreateRef<Scene>();
		context->ActiveScene->SetSceneFilepath("NULL");
		context->SelectedEntity = {};
		context->Resized = false;
	}

	void FileUtils::CopyDirectory(const std::filesystem::path& source, const std::filesystem::path& destination) {
		if (!std::filesystem::exists(source)) {
			FL_CORE_ERROR("Error: Source folder does not exist.");
			return;
		}

		if (!std::filesystem::exists(destination))
			std::filesystem::create_directory(destination);

		for (const auto& entry : std::filesystem::recursive_directory_iterator(source)) {
			// Get the path of the entry relative to the source folder
			const std::filesystem::path relative_path = std::filesystem::relative(entry.path(), source);

			// Construct the corresponding path in the destination folder
			const std::filesystem::path destination_path = destination / relative_path;

			if (std::filesystem::is_directory(entry)) {
				// If the entry is a subfolder, create a corresponding folder in the destination
				std::filesystem::create_directory(destination_path);
			} else {
				// If the entry is a file, copy it to the corresponding location in the destination
				std::ifstream infile(entry.path(), std::ios::binary);
				std::ofstream outfile(destination_path, std::ios::binary);

				if (infile.is_open() && outfile.is_open())
					outfile << infile.rdbuf();
			}
		}
	}

	void FileUtils::CreateScript(const std::filesystem::path& directory) { //currently not stable for typical users
		std::string templateContent = 
			"using System;\n"
			"using Flora;\n"
			"\n"
			"namespace Game\n"
			"{\n"
			"\tpublic class NewScript : Entity\n"
			"\t{\n"
			"\t\tvoid OnCreate()\n"
			"\t\t{\n"
			"\t\t\n"
			"\t\t}\n"
			"\n"
			"\t\tvoid OnDestroy()\n"
			"\t\t{\n"
			"\t\t\n"
			"\t\t}\n"
			"\n"
			"\t\tvoid OnUpdate(float ts)\n"
			"\t\t{\n"
			"\t\t\n"
			"\t\t}\n"
			"\t}\n"
			"}";
		std::string newfilename = "New Script.cs";
		int i = 0; 
		while (std::filesystem::exists(directory / newfilename)) {
			i++;
			newfilename = "New Script (" + std::to_string(i) + ").cs";
		}
		std::ofstream outfile(directory / newfilename);
		if (outfile.is_open()) {
			outfile << templateContent;
			outfile.close();
		} else {
			FL_CORE_ERROR("Unable to create file");
		}

		if (!Project::RegenerateScriptingProject()) {
			FL_CORE_ERROR("Unable to reload script into project");
		}
	}
	
	void FileUtils::CreateScene(const std::filesystem::path& directory, std::string filename) {
		std::string newfilename = filename + ".flora";
		int i = 0;
		while (std::filesystem::exists(directory / newfilename)) {
			i++;
			newfilename = filename +  " (" + std::to_string(i) + ").flora";
		}
		std::ofstream outfile(directory / newfilename);
		if (outfile.is_open()) {
			outfile.close();
		}
		else {
			FL_CORE_ERROR("Unable to create file");
		}
	}

	void FileUtils::SaveEditor(Ref<EditorParams> context) {
		Serializer::SerializeFile(EditorSerializer::Serialize(context), "Resources/Settings/fauna.fnproj"); 
	}

	void FileUtils::LoadEditor(Ref<EditorParams> context) {
		EditorSerializer::Deserialize(context); 
	}

	int FileUtils::ShellOpen(const std::filesystem::path path) {
		wchar_t winCurrDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, winCurrDir);
		SetCurrentDirectory(winCurrDir);
		std::string wcd = ConvertWCharToString(winCurrDir);
		std::string project_path = path.string();
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wideStr = converter.from_bytes(project_path);
		wcscat(winCurrDir, wideStr.c_str());
		HINSTANCE result = ShellExecute(nullptr, L"open", winCurrDir, nullptr, nullptr, SW_SHOWNORMAL);
		return (int)result;
	}
}