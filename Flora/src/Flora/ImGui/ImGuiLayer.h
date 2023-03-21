#pragma once
#include "Flora/Core/Layer.h"
#include "Flora/Events/KeyEvent.h"
#include "Flora/Events/MouseEvent.h"
#include "Flora/Events/ApplicationEvent.h"

namespace Flora {
	class FLORA_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();
		virtual void OnAttatch() override;
		virtual void OnDetatch() override;
		virtual void OnEvent(Event& e) override;
		void Begin();
		void End();
		void BlockEvents(bool block = true) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}