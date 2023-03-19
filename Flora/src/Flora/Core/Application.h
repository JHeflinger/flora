#pragma once
#include "Flora/Core/Base.h"
#include "Flora/Events/Event.h"
#include "Flora/Events/ApplicationEvent.h"
#include "Flora/Core/Window.h"
#include "Flora/Core/LayerStack.h"
#include "Flora/ImGui/ImGuiLayer.h"
#include "Flora/Core/Timestep.h"

namespace Flora {
	class Application {
	public:
		Application(const std::string& name = "Flora App");
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void Close();
		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}

