#include "flpch.h"
#include "Window.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Flora {
	Scope<Window> Window::Create(const WindowProps& props) {
		return CreateScope<WindowsWindow>(props);
	}
}