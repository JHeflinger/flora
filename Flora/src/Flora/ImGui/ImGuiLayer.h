#pragma once
#include "Flora/Layer.h"

namespace Flora {
	class FLORA_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();
		void OnAttatch();
		void OnDetatch();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		float m_Time = 0.0f;
	};
}