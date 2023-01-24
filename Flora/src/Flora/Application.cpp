#include "Application.h"
#include "Flora/Events/ApplicationEvent.h"
#include "Flora/Log.h"

namespace Flora {
	Application::Application() {

	}

	Application::~Application() {

	}

	void Application::Run() {
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication)) {
			FL_TRACE(e);
		}
		if (e.IsInCategory(EventCategoryInput)) {
			FL_TRACE(e);
		}
		while (true);
	}
}
