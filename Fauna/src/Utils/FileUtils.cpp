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
			Serializer::SerializeScene(context->ActiveScene, filepath);
		}
	}

	void FileUtils::SaveScene(Ref<EditorParams> context){
		std::string sceneFilepath = context->ActiveScene->GetSceneFilepath();
		if (sceneFilepath != "NULL") {
			Serializer::SerializeScene(context->ActiveScene, sceneFilepath);
		}
		else {
			SaveSceneAs(context);
		}
	}

	void FileUtils::OpenScene(Ref<EditorParams> context, const std::filesystem::path& path){
		NewScene(context);
		context->ActiveScene->SetSceneFilepath(path.string());
		Serializer::DeserializeScene(context->ActiveScene, path.string());
		Serializer::SerializeEditor(context);
	}

	void FileUtils::NewScene(Ref<EditorParams> context){
		context->ActiveScene = CreateRef<Scene>();
		context->ActiveScene->SetSceneFilepath("NULL");
		context->Resized = false;
	}
}