#pragma once
#include <string>

namespace Flora {
	class FileDialogs {
	public:
		// these will return an empty string if they are cancelled
		static std::string OpenFile(const char* filter);
		static std::string OpenFolder();
		static std::string SaveFile(const char* filter);
	};
}