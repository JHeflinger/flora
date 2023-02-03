#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Flora/Events/ApplicationEvent.h"
#include "Window.h"
#include "Flora/LayerStack.h"
#include "Flora/ImGui/ImGuiLayer.h"
#include "Flora/Renderer/Shader.h"
#include "Flora/Renderer/Buffer.h"
#include "Flora/Renderer/VertexArray.h"

namespace Flora {
	class FLORA_API Application {
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
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<Shader> m_SquareSH;
	private:
		static Application* s_Instance;
	};

	//To be defined in CLIENT
	Application* CreateApplication();
}

