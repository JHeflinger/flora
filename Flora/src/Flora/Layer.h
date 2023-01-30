#pragma once
#include "Flora/Core.h"
#include "Flora/Events/Event.h"

namespace Flora {
	class FLORA_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();
		virtual void OnAttatch() {}
		virtual void OnDetatch() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}
		inline const std::string& GetName() const{ return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}