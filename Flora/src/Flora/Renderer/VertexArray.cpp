#include "flpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Flora {
	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI()) {
		case RendererAPI::None:
			FL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexArray();
		}
		FL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}