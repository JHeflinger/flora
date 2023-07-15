#pragma once
#include "Flora/Core/Buffer.h"
#include <filesystem>

namespace Flora {
	class FileSystem {
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};

}