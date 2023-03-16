#pragma once
#include "Flora/Core/Base.h"
#include "Flora/Events/Event.h"
#include "Flora/Core/Timestep.h"

namespace Flora {
	class FLORA_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();
		virtual void OnAttatch() {}
		virtual void OnDetatch() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}
		inline const std::string& GetName() const{ return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}