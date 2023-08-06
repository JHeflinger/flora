#pragma once
#include "flpch.h"
#include "VisualUtils.h"

namespace Flora {
	static int64_t s_HoveredEntityHandle = -1;

#ifdef false
	static bool s_Initialized = false;
	static Ref<Framebuffer> s_Framebuffer;

	void VisualUtils::SetVisualContext(Ref<Framebuffer> fbr) {
		s_Framebuffer = fbr;
		s_Initialized = true;
	}

	bool VisualUtils::IsContextValid() {
		return s_Initialized;
	}

	//TODO: this does not work in editor, so we have to grab via viewport. Implement and make sure this works when implementing shipping game!!

	int64_t VisualUtils::GetHoveredEntity() {
		int pixelData = s_Framebuffer->ReadPixel(1, mouseX, mouseY);
		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
			
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_EditorContext->ActiveScene.get());
		}
	}
#endif

	int64_t VisualUtils::GetHoveredEntity() {
		return s_HoveredEntityHandle;
	}

	void VisualUtils::SetHoveredEntity(int64_t entityHandle) {
		s_HoveredEntityHandle = entityHandle;
	}
}