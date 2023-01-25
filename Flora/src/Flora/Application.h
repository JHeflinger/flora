#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Flora/Events/ApplicationEvent.h"
#include "Window.h"
#include "Flora/LayerStack.h"

namespace Flora {
	class FLORA_API Application {
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}

