#pragma once
#include "Flora/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Flora{
	class OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}

