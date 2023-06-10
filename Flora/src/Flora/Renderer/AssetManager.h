#pragma once
#include "Flora/Renderer/Texture.h"
#include <map>

namespace Flora {
	class AssetManager {
	public:
		AssetManager();
		~AssetManager();
		bool AddTexture(std::string texPath); //returns false if texture was already added
		void RemoveTexture(std::string texPath);
		void ClearTextures();
		Ref<Texture2D> GetTexture(std::string texPath);
	private:
		std::map<std::string, Ref<Texture2D>> m_TextureMap;
	};
}