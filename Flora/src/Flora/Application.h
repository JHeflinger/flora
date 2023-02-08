#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Flora/Events/ApplicationEvent.h"
#include "Window.h"
#include "Flora/LayerStack.h"
#include "Flora/ImGui/ImGuiLayer.h"
#include "Flora/Core/Timestep.h"

namespace Flora {
	class Application {
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}

