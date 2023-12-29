#pragma once
#include "Flora/Renderer/Texture.h"
#ifdef WIN_BUILD_ONLY
#include "Flora/Renderer/Font.h"
#endif
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
	#ifdef WIN_BUILD_ONLY
	public:
		static bool AddFont(std::string fontPath);
		static void RemoveFont(std::string fontPath);
		static void ClearFonts();
		static Ref<Font> GetFont(std::string fontPath);
	#endif
	};
}