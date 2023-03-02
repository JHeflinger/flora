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
		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}