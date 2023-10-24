#pragma once
#include "Flora/Renderer/Texture.h"
#include "Flora/Audio/AudioCommand.h"

namespace Flora {
	class AssetManager {
	public:
		static void Init();
	public:
		static bool AddTexture(std::string texPath);
		static void RemoveTexture(std::string texPath);
		static void ClearTextures();
		static Ref<Texture2D> GetTexture(std::string texPath);
	public:
		static bool AddAudio(std::string audioPath);
		static void RemoveAudio(std::string audioPath);
		static void ClearAudios();
		static Ref<Audio> GetAudio(std::string audioPath);
	};
}