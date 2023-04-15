#include "flpch.h"
#include "FileUtils.h"

namespace Flora {
	void FileUtils::OpenScene(Ref<EditorParams> context){}
	void FileUtils::SaveSceneAs(Ref<EditorParams> context){}
	void FileUtils::SaveScene(Ref<EditorParams> context){}

	void FileUtils::OpenScene(Ref<EditorParams> context, const std::filesystem::path& path){
		FL_CORE_INFO("openscene called");
	}

	void FileUtils::NewScene(Ref<EditorParams> context){
		
	}
}