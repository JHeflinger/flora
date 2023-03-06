#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"

namespace Flora {
	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();
		static void DrawQuad(const glm::vec2& position, 
							 const glm::vec2& size,
							 const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
							 const Ref<Texture2D>& texture = nullptr,
							 float rotation = 0.0f,
							 float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, 
							 const glm::vec2& size,
							 const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
							 const Ref<Texture2D>& texture = nullptr,
							 float rotation = 0.0f,
							 float tilingFactor = 1.0f);
	};
}