#include "flpch.h"
#include "Texture.h"
#include "Flora/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Flora {
	Ref<Texture2D> Texture2D::Create(const TextureSpecification& spec) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			FL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(spec);
		}
		FL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			FL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
		}
		FL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}