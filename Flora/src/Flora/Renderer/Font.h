#pragma once

namespace Flora {
	struct FontData;

	class Font {
	public:
		Font(const std::filesystem::path& filepath);
		~Font();
	private:
		FontData* m_Data;
	};
}