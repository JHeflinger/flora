#pragma once
#include "Flora/Renderer/OrthographicCamera.h"
#include "Flora/Renderer/Texture.h"
#include "Flora/Renderer/SubTexture2D.h"
#include "Flora/Renderer/Camera.h"
#include "Flora/Renderer/EditorCamera.h"
#ifdef WIN_BUILD_ONLY
#include "Flora/Renderer/Font.h"
#endif
#include "Flora/Scene/Components.h"
#include "Flora/Core/AssetManager.h"

namespace Flora {
	#ifdef WIN_BUILD_ONLY
	struct TextConfig {
		std::string TextString;
		float Kerning = 0.0f;
		float LineSpacing = 0.0f;
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
		FontAlignment Alignment = FontAlignment::LEFT;
		Ref<Font> Font = nullptr;
		int EntityID = -1;
	};
	#endif

	class Renderer2D {
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const glm::mat4 viewProjection);
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
							 const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
							 int entityID = -1);
		static void DrawQuad(const glm::mat4& transform,
							 const Ref<Texture2D>& texture,
							 const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
							 float tilingFactor = 1.0f,
							 int entityID = -1);
		static void DrawQuad(const glm::mat4& transform,
							 const Ref<SubTexture2D>& subtexture,
							 const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f },
							 float tilingFactor = 1.0f,
							 int entityID = -1);
		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float rotation = 0.0f, int entityID = -1);
		static void DrawRect(const glm::mat4& transform, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, int entityID = -1);
		static void DrawSprite(Timestep ts, const glm::mat4& transform, SpriteRendererComponent& src, int entityID = -1);
		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float thickness = 1.0f, float fade = 0.005f, int entityID = -1);
		static void DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, int entityID = -1);
		#ifdef WIN_BUILD_ONLY
		static void DrawString(const TextConfig& config);
		static void DrawString(TextComponent& tc, Entity entity);
		#endif
		static float GetLineWidth();
		static void SetLineWidth(float width);

		// Stats
		struct Statistics {
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t CircleCount = 0;
			uint32_t LineCount = 0;

			uint32_t GetTotalVertexCount() { return (QuadCount + CircleCount) * 4 + (LineCount * 2); }
			uint32_t GetTotalIndexCount() { return (QuadCount + CircleCount) * 6; }
		};
		static Statistics GetStats();
		static void ResetStats();
	private:
		static void StartBatch();
		static void NextBatch();
	};
}