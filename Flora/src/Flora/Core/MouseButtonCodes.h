#pragma once

namespace Flora
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define FL_MOUSE_BUTTON_0      ::Flora::Mouse::Button0
#define FL_MOUSE_BUTTON_1      ::Flora::Mouse::Button1
#define FL_MOUSE_BUTTON_2      ::Flora::Mouse::Button2
#define FL_MOUSE_BUTTON_3      ::Flora::Mouse::Button3
#define FL_MOUSE_BUTTON_4      ::Flora::Mouse::Button4
#define FL_MOUSE_BUTTON_5      ::Flora::Mouse::Button5
#define FL_MOUSE_BUTTON_6      ::Flora::Mouse::Button6
#define FL_MOUSE_BUTTON_7      ::Flora::Mouse::Button7
#define FL_MOUSE_BUTTON_LAST   ::Flora::Mouse::ButtonLast
#define FL_MOUSE_BUTTON_LEFT   ::Flora::Mouse::ButtonLeft
#define FL_MOUSE_BUTTON_RIGHT  ::Flora::Mouse::ButtonRight
#define FL_MOUSE_BUTTON_MIDDLE ::Flora::Mouse::ButtonMiddle
