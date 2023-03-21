#pragma once

namespace Flora
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define FL_KEY_SPACE           ::Flora::Key::Space
#define FL_KEY_APOSTROPHE      ::Flora::Key::Apostrophe    /* ' */
#define FL_KEY_COMMA           ::Flora::Key::Comma         /* , */
#define FL_KEY_MINUS           ::Flora::Key::Minus         /* - */
#define FL_KEY_PERIOD          ::Flora::Key::Period        /* . */
#define FL_KEY_SLASH           ::Flora::Key::Slash         /* / */
#define FL_KEY_0               ::Flora::Key::D0
#define FL_KEY_1               ::Flora::Key::D1
#define FL_KEY_2               ::Flora::Key::D2
#define FL_KEY_3               ::Flora::Key::D3
#define FL_KEY_4               ::Flora::Key::D4
#define FL_KEY_5               ::Flora::Key::D5
#define FL_KEY_6               ::Flora::Key::D6
#define FL_KEY_7               ::Flora::Key::D7
#define FL_KEY_8               ::Flora::Key::D8
#define FL_KEY_9               ::Flora::Key::D9
#define FL_KEY_SEMICOLON       ::Flora::Key::Semicolon     /* ; */
#define FL_KEY_EQUAL           ::Flora::Key::Equal         /* = */
#define FL_KEY_A               ::Flora::Key::A
#define FL_KEY_B               ::Flora::Key::B
#define FL_KEY_C               ::Flora::Key::C
#define FL_KEY_D               ::Flora::Key::D
#define FL_KEY_E               ::Flora::Key::E
#define FL_KEY_F               ::Flora::Key::F
#define FL_KEY_G               ::Flora::Key::G
#define FL_KEY_H               ::Flora::Key::H
#define FL_KEY_I               ::Flora::Key::I
#define FL_KEY_J               ::Flora::Key::J
#define FL_KEY_K               ::Flora::Key::K
#define FL_KEY_L               ::Flora::Key::L
#define FL_KEY_M               ::Flora::Key::M
#define FL_KEY_N               ::Flora::Key::N
#define FL_KEY_O               ::Flora::Key::O
#define FL_KEY_P               ::Flora::Key::P
#define FL_KEY_Q               ::Flora::Key::Q
#define FL_KEY_R               ::Flora::Key::R
#define FL_KEY_S               ::Flora::Key::S
#define FL_KEY_T               ::Flora::Key::T
#define FL_KEY_U               ::Flora::Key::U
#define FL_KEY_V               ::Flora::Key::V
#define FL_KEY_W               ::Flora::Key::W
#define FL_KEY_X               ::Flora::Key::X
#define FL_KEY_Y               ::Flora::Key::Y
#define FL_KEY_Z               ::Flora::Key::Z
#define FL_KEY_LEFT_BRACKET    ::Flora::Key::LeftBracket   /* [ */
#define FL_KEY_BACKSLASH       ::Flora::Key::Backslash     /* \ */
#define FL_KEY_RIGHT_BRACKET   ::Flora::Key::RightBracket  /* ] */
#define FL_KEY_GRAVE_ACCENT    ::Flora::Key::GraveAccent   /* ` */
#define FL_KEY_WORLD_1         ::Flora::Key::World1        /* non-US #1 */
#define FL_KEY_WORLD_2         ::Flora::Key::World2        /* non-US #2 */

/* Function keys */
#define FL_KEY_ESCAPE          ::Flora::Key::Escape
#define FL_KEY_ENTER           ::Flora::Key::Enter
#define FL_KEY_TAB             ::Flora::Key::Tab
#define FL_KEY_BACKSPACE       ::Flora::Key::Backspace
#define FL_KEY_INSERT          ::Flora::Key::Insert
#define FL_KEY_DELETE          ::Flora::Key::Delete
#define FL_KEY_RIGHT           ::Flora::Key::Right
#define FL_KEY_LEFT            ::Flora::Key::Left
#define FL_KEY_DOWN            ::Flora::Key::Down
#define FL_KEY_UP              ::Flora::Key::Up
#define FL_KEY_PAGE_UP         ::Flora::Key::PageUp
#define FL_KEY_PAGE_DOWN       ::Flora::Key::PageDown
#define FL_KEY_HOME            ::Flora::Key::Home
#define FL_KEY_END             ::Flora::Key::End
#define FL_KEY_CAPS_LOCK       ::Flora::Key::CapsLock
#define FL_KEY_SCROLL_LOCK     ::Flora::Key::ScrollLock
#define FL_KEY_NUM_LOCK        ::Flora::Key::NumLock
#define FL_KEY_PRINT_SCREEN    ::Flora::Key::PrintScreen
#define FL_KEY_PAUSE           ::Flora::Key::Pause
#define FL_KEY_F1              ::Flora::Key::F1
#define FL_KEY_F2              ::Flora::Key::F2
#define FL_KEY_F3              ::Flora::Key::F3
#define FL_KEY_F4              ::Flora::Key::F4
#define FL_KEY_F5              ::Flora::Key::F5
#define FL_KEY_F6              ::Flora::Key::F6
#define FL_KEY_F7              ::Flora::Key::F7
#define FL_KEY_F8              ::Flora::Key::F8
#define FL_KEY_F9              ::Flora::Key::F9
#define FL_KEY_F10             ::Flora::Key::F10
#define FL_KEY_F11             ::Flora::Key::F11
#define FL_KEY_F12             ::Flora::Key::F12
#define FL_KEY_F13             ::Flora::Key::F13
#define FL_KEY_F14             ::Flora::Key::F14
#define FL_KEY_F15             ::Flora::Key::F15
#define FL_KEY_F16             ::Flora::Key::F16
#define FL_KEY_F17             ::Flora::Key::F17
#define FL_KEY_F18             ::Flora::Key::F18
#define FL_KEY_F19             ::Flora::Key::F19
#define FL_KEY_F20             ::Flora::Key::F20
#define FL_KEY_F21             ::Flora::Key::F21
#define FL_KEY_F22             ::Flora::Key::F22
#define FL_KEY_F23             ::Flora::Key::F23
#define FL_KEY_F24             ::Flora::Key::F24
#define FL_KEY_F25             ::Flora::Key::F25

/* Keypad */
#define FL_KEY_KP_0            ::Flora::Key::KP0
#define FL_KEY_KP_1            ::Flora::Key::KP1
#define FL_KEY_KP_2            ::Flora::Key::KP2
#define FL_KEY_KP_3            ::Flora::Key::KP3
#define FL_KEY_KP_4            ::Flora::Key::KP4
#define FL_KEY_KP_5            ::Flora::Key::KP5
#define FL_KEY_KP_6            ::Flora::Key::KP6
#define FL_KEY_KP_7            ::Flora::Key::KP7
#define FL_KEY_KP_8            ::Flora::Key::KP8
#define FL_KEY_KP_9            ::Flora::Key::KP9
#define FL_KEY_KP_DECIMAL      ::Flora::Key::KPDecimal
#define FL_KEY_KP_DIVIDE       ::Flora::Key::KPDivide
#define FL_KEY_KP_MULTIPLY     ::Flora::Key::KPMultiply
#define FL_KEY_KP_SUBTRACT     ::Flora::Key::KPSubtract
#define FL_KEY_KP_ADD          ::Flora::Key::KPAdd
#define FL_KEY_KP_ENTER        ::Flora::Key::KPEnter
#define FL_KEY_KP_EQUAL        ::Flora::Key::KPEqual

#define FL_KEY_LEFT_SHIFT      ::Flora::Key::LeftShift
#define FL_KEY_LEFT_CONTROL    ::Flora::Key::LeftControl
#define FL_KEY_LEFT_ALT        ::Flora::Key::LeftAlt
#define FL_KEY_LEFT_SUPER      ::Flora::Key::LeftSuper
#define FL_KEY_RIGHT_SHIFT     ::Flora::Key::RightShift
#define FL_KEY_RIGHT_CONTROL   ::Flora::Key::RightControl
#define FL_KEY_RIGHT_ALT       ::Flora::Key::RightAlt
#define FL_KEY_RIGHT_SUPER     ::Flora::Key::RightSuper
#define FL_KEY_MENU            ::Flora::Key::Menu
