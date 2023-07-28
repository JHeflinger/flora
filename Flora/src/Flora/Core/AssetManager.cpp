#include "flpch.h"
#include "Flora/Core/AssetManager.h"

namespace Flora {
	std::map<std::string, Ref<Texture2D>> s_TextureMap;
	std::map<std::string, Ref<Audio>> s_AudioMap;

	void AssetManager::Init() {
		ClearAudios();
		ClearTextures();
	}

	bool AssetManager::AddTexture(std::string texPath) {
		if (s_TextureMap.find(texPath) != s_TextureMap.end()) return false;
		s_TextureMap[texPath] = Texture2D::Create(texPath);
		return true;
	}

	void AssetManager::RemoveTexture(std::string texPath) {
		s_TextureMap.erase(texPath);
	}

	void AssetManager::ClearTextures() {
		s_TextureMap.clear();
	}

	Ref<Texture2D> AssetManager::GetTexture(std::string texPath) {
		return s_TextureMap[texPath];
	}

	bool AssetManager::AddAudio(std::string audioPath) {
		if (s_AudioMap.find(audioPath) != s_AudioMap.end()) return false;
		s_AudioMap[audioPath] = CreateScope<Audio>(AudioCommand::LoadWAV(audioPath));
		return true;
	}

	void AssetManager::RemoveAudio(std::string audioPath) {
		s_AudioMap.erase(audioPath);
	}

	void AssetManager::ClearAudios() {
		s_AudioMap.clear();
	}

	Ref<Audio> AssetManager::GetAudio(std::string audioPath) {
		return s_AudioMap[audioPath];
	}
}