#pragma once

namespace Flora {
	struct ViewportInfo {
		float width = 0.0f;
		float height = 0.0f;
		float mouseX = 0.0f;
		float mouseY = 0.0f;
	};

	class VisualUtils {
	public:
		static int64_t GetHoveredEntity();
		static void SetHoveredEntity(int64_t entityHandle);
		static ViewportInfo& GetViewportInfo();
		// ideas for more utils: hide mouse? set mouse cursor texture?
	};
}