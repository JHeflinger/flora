#include "flpch.h"
#include "FileUtils.h"
#include "Flora/Utils/PlatformUtils.h"
#include "EditorSerializer.h"
#include "Flora/Utils/Serializer.h"

namespace Flora {
	void FileUtils::OpenScene(Ref<EditorParams> context){
		std::string filepath = FileDialogs::OpenFile("Flora Scene (*.flora)\0*.flora\0");
		if (!filepath.empty()) {
			OpenScene(context, filepath);
		}
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
		Serializer::SerializeFile(sceneContent, "temp/tempScene.flora");
		std::string editorContent = EditorSerializer::Serialize(context);
		Serializer::SerializeFile(editorContent, "temp/tempEditorSettings.fnproj");
	}

	void FileUtils::OpenTempScene(Ref<EditorParams> context) {
		EditorSerializer::Deserialize(context, "temp/tempEditorSettings.fnproj");
		std::string sceneFilepath = context->ActiveScene->GetSceneFilepath();
		NewScene(context);
		context->ActiveScene->SetSceneFilepath(sceneFilepath);
		Serializer::DeserializeScene(context->ActiveScene, "temp/tempscene.flora");
	}

	void FileUtils::OpenScene(Ref<EditorParams> context, const std::filesystem::path& path){
		NewScene(context);
		context->ActiveScene->SetSceneFilepath(path.string());
		context->SelectedEntity = {};
		Serializer::DeserializeScene(context->ActiveScene, path.string());
		std::string editorContent = EditorSerializer::Serialize(context);
		Serializer::SerializeFile(editorContent, "assets/settings/fauna.fnproj");
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
			"#pragma once\n"
			"#include \"Flora/Scene/ScriptableEntity.h\"\n"
			"\n"
			"namespace Flora {\n"
			"\tclass TEMPLATE : public ScriptableEntity {\n"
			"\tpublic:\n"
			"\t\tvoid OnCreate() {\n"
			"\t\t\n"
			"\t\t}\n"
			"\n"
			"\t\tvoid OnDestroy() {\n"
			"\t\t\n"
			"\t\t}\n"
			"\n"
			"\t\tvoid OnUpdate(Timestep ts) {\n"
			"\t\t\n"
			"\t\t}\n"
			"\t};\n"
			"}";
		std::string newfilename = "New Script.h";
		int i = 0; 
		while (std::filesystem::exists(directory / newfilename)) {
			i++;
			newfilename = "New Script (" + std::to_string(i) + ").h";
		}
		std::ofstream outfile(directory / newfilename);
		if (outfile.is_open()) {
			outfile << templateContent;
			outfile.close();
		} else {
			FL_CORE_ERROR("Unable to create file");
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
		Serializer::SerializeFile(EditorSerializer::Serialize(context), "assets/settings/fauna.fnproj"); 
	}

	void FileUtils::LoadEditor(Ref<EditorParams> context) {
		EditorSerializer::Deserialize(context); 
	}
}