#include "flpch.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>

namespace Flora {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) 
		: m_WindowHandle(windowHandle) {
		FL_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init() {
		FL_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		FL_CORE_ASSERT(status, "Failed to initialize Glad!");
		FL_CORE_INFO("OpenGL Info:\n\t\t   Vendor: {0}\n\t\t   Renderer: {1}\n\t\t   Version: {2}",
			(char*)glGetString(GL_VENDOR),
			(char*)glGetString(GL_RENDERER),
			(char*)glGetString(GL_VERSION));

		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		FL_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Flora requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers() {
		FL_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}
