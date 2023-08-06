#pragma once

namespace Flora {
	class VisualUtils {
	public:
		static int64_t GetHoveredEntity();
		static void SetHoveredEntity(int64_t entityHandle);
		// ideas for more utils: hide mouse? set mouse cursor texture?
	};
}