#pragma once
#include "Flora/Renderer/Texture.h"
#include "Flora/Audio/AudioCommand.h"
#include <map>

namespace Flora {
	class AssetManager {
	public:
		AssetManager();
		~AssetManager();
	public:
		bool AddTexture(std::string texPath); //returns false if texture was already added
		void RemoveTexture(std::string texPath);
		void ClearTextures();
		Ref<Texture2D> GetTexture(std::string texPath);
	public:
		bool AddAudio(std::string audioPath); //returns false if audio was already added
		void RemoveAudio(std::string audioPath);
		void ClearAudios();
		Ref<Audio> GetAudio(std::string audioPath);
	private:
		std::map<std::string, Ref<Texture2D>> m_TextureMap;
		std::map<std::string, Ref<Audio>> m_AudioMap;
	};
}