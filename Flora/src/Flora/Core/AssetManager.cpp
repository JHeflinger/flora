#include "flpch.h"
#include "Flora/Core/AssetManager.h"

namespace Flora {
	AssetManager::AssetManager() {

	}

	AssetManager::~AssetManager() {
		// note: might be a minor memory leak here?
	}

	bool AssetManager::AddTexture(std::string texPath) {
		if (m_TextureMap.find(texPath) != m_TextureMap.end()) return false;
		m_TextureMap[texPath] = Texture2D::Create(texPath);
		return true;
	}

	void AssetManager::RemoveTexture(std::string texPath) {
		m_TextureMap.erase(texPath);
	}

	void AssetManager::ClearTextures() {
		m_TextureMap.clear();
	}

	Ref<Texture2D> AssetManager::GetTexture(std::string texPath) {
		return m_TextureMap[texPath];
	}

	bool AssetManager::AddAudio(std::string audioPath) {
		if (m_AudioMap.find(audioPath) != m_AudioMap.end()) return false;
		m_AudioMap[audioPath] = CreateScope<Audio>(AudioCommand::LoadWAV(audioPath));
		return true;
	}

	void AssetManager::RemoveAudio(std::string audioPath) {
		m_AudioMap.erase(audioPath);
	}

	void AssetManager::ClearAudios() {
		m_AudioMap.clear();
	}

	Ref<Audio> AssetManager::GetAudio(std::string audioPath) {
		return m_AudioMap[audioPath];
	}
}