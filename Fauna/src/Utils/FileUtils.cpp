#include "flpch.h"
#include "FileUtils.h"
#include "Flora/Utils/Serializer.h"
#include "Flora/Utils/PlatformUtils.h"

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

	void FileUtils::OpenScene(Ref<EditorParams> context, const std::filesystem::path& path){
		NewScene(context);
		context->ActiveScene->SetSceneFilepath(path.string());
		context->SelectedEntity = {};
		Serializer::DeserializeScene(context->ActiveScene, path.string());
		std::string editorContent = Serializer::SerializeEditor(context);
		Serializer::SerializeFile(editorContent, "assets/settings/fauna.fnproj");
	}

	void FileUtils::NewScene(Ref<EditorParams> context){
		context->ActiveScene = CreateRef<Scene>();
		context->ActiveScene->SetSceneFilepath("NULL");
		context->SelectedEntity = {};
		context->Resized = false;
	}
}