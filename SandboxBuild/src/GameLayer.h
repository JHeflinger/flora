#pragma once
#include "Flora.h"
#include "Flora/Core/Layer.h"
#include "SceneManager/SceneManager.h"

namespace Flora {
	class GameLayer : public Layer {
	public:
		GameLayer();
		virtual ~GameLayer() = default;
		virtual bool RequestCloseProtection() override { return true; }
		virtual bool ConfirmClose() override { return true; }
		virtual void ProcessWindowClose() override {};
		virtual void OnAttatch() override;
		virtual void OnDetatch() override;
		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		SceneManager m_SceneManager;
	};
}