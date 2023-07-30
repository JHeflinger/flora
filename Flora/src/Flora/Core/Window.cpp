#include "flpch.h"
#include "Window.h"

#ifdef FL_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Flora {
	Scope<Window> Window::Create(const WindowProps& props) {
		#ifdef FL_PLATFORM_WINDOWS
			return CreateScope<WindowsWindow>(props);
		#else
			FL_CORE_ASSERT(false, "Unknown platform!");
			return nullptr;
		#endif
	}
}