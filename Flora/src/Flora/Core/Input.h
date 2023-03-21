#pragma once
#include "Flora/Core/Base.h"
#include "Flora/Core/KeyCodes.h"
#include "Flora/Core/MouseButtonCodes.h"

namespace Flora {
	class FLORA_API Input {
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();
	};
}