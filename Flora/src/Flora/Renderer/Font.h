#pragma once
#include "Flora/Renderer/Texture.h"

namespace Flora {
	struct FontData;

	class Font {
	public:
		Font(const std::filesystem::path& filepath);
		~Font();
		FontData* GetFontData() const { return m_Data; }
		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }
		static Ref<Font> GetDefault();
	private:
		FontData* m_Data;
		Ref<Texture2D> m_AtlasTexture;
	};
}