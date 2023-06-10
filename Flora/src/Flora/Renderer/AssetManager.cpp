#include "flpch.h"
#include "Flora/Renderer/AssetManager.h"

namespace Flora {
	AssetManager::AssetManager() {

	}

	AssetManager::~AssetManager() {
		// note: might be a minor memory leak here?
	}

	bool AssetManager::AddTexture(std::string texPath) {
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
}