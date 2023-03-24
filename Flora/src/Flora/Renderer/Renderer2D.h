#pragma once
#include "Flora/Renderer/OrthographicCamera.h"
#include "Flora/Renderer/Texture.h"
#include "Flora/Renderer/SubTexture2D.h"
#include "Flora/Renderer/Camera.h"

namespace Flora {
	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void EndScene();
		static void Flush();
		static void DrawQuad(const glm::vec2& position, 
							 const glm::vec2& size,
							 const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
							 float rotation = 0.0f);
		static void DrawQuad(const glm::vec3& position, 
							 const glm::vec2& size,
							 const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
							 float rotation = 0.0f);
		static void DrawQuad(const glm::vec2& position, 
							 const glm::vec2& size,
							 const Ref<Texture2D>& texture,
							 const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
							 float rotation = 0.0f,
							 float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, 
							 const glm::vec2& size,
							 const Ref<Texture2D>& texture,
							 const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
							 float rotation = 0.0f,
							 float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec2& position, 
							 const glm::vec2& size,
							 const Ref<SubTexture2D>& subtexture,
							 const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
							 float rotation = 0.0f,
							 float tilingFactor = 1.0f);
		static void DrawQuad(const glm::vec3& position, 
							 const glm::vec2& size,
							 const Ref<SubTexture2D>& subtexture,
							 const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
							 float rotation = 0.0f,
							 float tilingFactor = 1.0f);
		static void DrawQuad(const glm::mat4& transform,
							 const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::mat4& transform,
							 const Ref<Texture2D>& texture,
							 const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
							 float tilingFactor = 1.0f);

		// Stats
		struct Statistics {
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static Statistics GetStats();
		static void ResetStats();
	private:
		static void FlushAndReset();
	};
}