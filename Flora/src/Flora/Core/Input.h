#pragma once
#include "Flora/Core/Base.h"
#include "Flora/Core/KeyCodes.h"
#include "Flora/Core/MouseCodes.h"
#include "glm/glm.hpp"

namespace Flora {
	class FLORA_API Input {
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		static float GetMouseX();
		static float GetMouseY();
		static glm::vec2 GetMousePosition();
	};
}