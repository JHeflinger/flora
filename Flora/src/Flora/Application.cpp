#include "flpch.h"
#include "Application.h"
#include "Flora/Log.h"
#include <glad/glad.h>
#include "Input.h"

namespace Flora {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		FL_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application() {

	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttatch();
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttatch();
	}

	void Application::OnEvent(Event& e) {
		EventDispacher dispacher(e);
		dispacher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run() {
		while (m_Running) {
			glClearColor(1, 1, 1, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			auto [x, y] = Input::GetMousePosition();
			FL_CORE_TRACE("{0}, {1}", x, y);

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}
