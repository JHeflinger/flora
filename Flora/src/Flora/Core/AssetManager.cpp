#include "flpch.h"
#include "AssetManager.h"
#include "Flora/Project/Project.h"

namespace Flora {
	std::map<std::string, Ref<Texture2D>> s_TextureMap;
	std::map<std::string, Ref<Audio>> s_AudioMap;
	std::map<std::string, Ref<Font>> s_FontMap;

	static std::string GetTruePath(std::string texPath) {
		if (texPath.size() > 0)
			if (texPath[0] == '$')
				texPath = (Project::GetAssetDirectory() / texPath.substr(2)).string();
		return texPath;
	}

	void AssetManager::Init() {
		ClearAudios();
		ClearTextures();
		ClearFonts();
	}

	bool AssetManager::AddTexture(std::string texPath) {
		if (s_TextureMap.find(texPath) != s_TextureMap.end()) return false;
		texPath = GetTruePath(texPath);
		std::ifstream file(texPath);
		if (!file)
			s_TextureMap[texPath] = Texture2D::Create("resources/Defaults/invalid.png");
		else
			s_TextureMap[texPath] = Texture2D::Create(texPath);
		return true;
	}

	void AssetManager::RemoveTexture(std::string texPath) {
		texPath = GetTruePath(texPath);
		s_TextureMap.erase(texPath);
	}

	void AssetManager::ClearTextures() {
		s_TextureMap.clear();
	}

	Ref<Texture2D> AssetManager::GetTexture(std::string texPath) {
		texPath = GetTruePath(texPath);
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

	bool AssetManager::AddFont(std::string fontPath) {
		if (s_FontMap.find(fontPath) != s_FontMap.end()) return false;
		fontPath = GetTruePath(fontPath);
		std::ifstream file(fontPath);
		if (!file)
			s_FontMap[fontPath] = Font::GetDefault();
		else
			s_FontMap[fontPath] = CreateRef<Font>(fontPath);
		return true;
	}

	void AssetManager::RemoveFont(std::string fontPath) {
		fontPath = GetTruePath(fontPath);
		s_FontMap.erase(fontPath);
	}

	void AssetManager::ClearFonts() {
		s_FontMap.clear();
	}

	Ref<Font> AssetManager::GetFont(std::string fontPath) {
		fontPath = GetTruePath(fontPath);
		return s_FontMap[fontPath];
	}
}