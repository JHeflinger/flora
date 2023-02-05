#include "flpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Flora {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}