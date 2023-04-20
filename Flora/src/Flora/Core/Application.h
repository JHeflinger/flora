#pragma once
#include "Flora/Core/Base.h"
#include "Flora/Events/Event.h"
#include "Flora/Events/ApplicationEvent.h"
#include "Flora/Core/Window.h"
#include "Flora/Core/LayerStack.h"
#include "Flora/ImGui/ImGuiLayer.h"
#include "Flora/Core/Timestep.h"

namespace Flora {
	struct ApplicationCommandLineArgs {
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const {
			FL_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	class Application {
	public:
		Application(const std::string& name = "Flora App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void Close();
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		inline static Application& Get() { return *s_Instance; }
		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		ApplicationCommandLineArgs m_CommandLineArgs;
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		bool m_ProtectClose = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication(ApplicationCommandLineArgs args);
}

