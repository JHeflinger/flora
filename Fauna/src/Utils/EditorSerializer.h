#pragma once
#include "Structures.h"

namespace Flora {
	class EditorSerializer {
	public:
		static std::string Serialize(Ref<EditorParams> params);
		static bool Deserialize(Ref<EditorParams> params, const std::string& filepath = "Resources/Settings/fauna.fnproj");
	};
}
