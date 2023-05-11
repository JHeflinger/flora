#include "flpch.h"
#include "Flora/Renderer/Renderer2D.h"
#include "Flora/Renderer/VertexArray.h"
#include "Flora/Renderer/Shader.h"
#include "Flora/Renderer/RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Flora {
	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// Editor-only
		int EntityID;
	};

	struct Renderer2DData {
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 is the white texture

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init() {
		FL_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Int, "a_EntityID" }
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;
			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}

		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown() {
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		FL_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera) {
		FL_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjection());

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) {
		FL_PROFILE_FUNCTION();

		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		StartBatch();
	}

	void Renderer2D::EndScene() {
		FL_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::StartBatch() {
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::Flush() {
		if (s_Data.QuadIndexCount == 0)
			return; // Nothing to draw

		uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

		// Bind textures
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
		s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::NextBatch() {
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float rotation, float tilingFactor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, color, rotation, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float rotation, float tilingFactor) {
		FL_PROFILE_FUNCTION();

		glm::mat4 transform;
		if (rotation == 0.0f) {
			transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		}
		else {
			transform = glm::translate(glm::mat4(1.0f), position)
				* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		}

		DrawQuad(transform, texture, color, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& color, float rotation, float tilingFactor) {
		DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, color, rotation, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& color, float rotation, float tilingFactor) {
		FL_PROFILE_FUNCTION();

		const Ref<Texture2D> texture = subtexture->GetTexture();
		const glm::vec2* textureCoords = subtexture->GetTexCoords();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) NextBatch();

		glm::mat4 transform;
		if (rotation == 0.0f) {
			transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		}
		else {
			transform = glm::translate(glm::mat4(1.0f), position)
				* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		}

		float textureIndex = 0.0f;
		if (texture != nullptr) {
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
				if (*s_Data.TextureSlots[i].get() == *texture.get()) {
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f) {
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}
		}

		constexpr size_t quadVertexCount = 4;

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotation) {
		DrawQuad({ position.x, position.y, 0 }, size, color, rotation);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation) {
		FL_PROFILE_FUNCTION();

		glm::mat4 transform;
		if (rotation == 0.0f) {
			transform = glm::translate(glm::mat4(1.0f), position)
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		}
		else {
			transform = glm::translate(glm::mat4(1.0f), position)
				* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		}

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID) {
		FL_PROFILE_FUNCTION();

		float textureIndex = 0.0f;
		float tilingFactor = 1.0f;

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& color, float tilingFactor, int entityID) {
		FL_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) NextBatch();

		float textureIndex = 0.0f;
		if (texture != nullptr) {
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
				s_Data.TextureSlots[i];
				texture;
				if (s_Data.TextureSlots[i].get() == texture.get()) {
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f) {
				if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) NextBatch();

				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}
		}

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = {
			{0.0f, 0.0f},
			{1.0f, 0.0f},
			{1.0f, 1.0f},
			{0.0f, 1.0f}
		};

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subtexture, const glm::vec4& color, float tilingFactor, int entityID) {
		FL_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) NextBatch();

		const Ref<Texture2D> texture = subtexture->GetTexture();
		const glm::vec2* textureCoords = subtexture->GetTexCoords();

		float textureIndex = 0.0f;
		if (texture != nullptr) {
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
				if (*s_Data.TextureSlots[i].get() == *texture.get()) {
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f) {
				if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) NextBatch();

				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}
		}

		constexpr size_t quadVertexCount = 4;

		for (size_t i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& src, AssetManager* am, int entityID) {
		Ref<Texture2D> texture = nullptr;
		if (src.Path != "NULL") texture = am->GetTexture(src.Path);
		if (src.Type == SpriteRendererComponent::SpriteType::SINGLE) {
			if (texture)
				DrawQuad(transform, texture, src.Color, src.TilingFactor, entityID);
			else
				DrawQuad(transform, src.Color, entityID);
		} else if (src.Type == SpriteRendererComponent::SpriteType::SUBTEXTURE) {
			if (texture) {
				glm::vec2 coords = { src.ColumnCoordinate - 1, (src.Rows - 1) - (src.RowCoordinate - 1) };
				glm::vec2 cellSize = { texture->GetWidth() / src.Columns, texture->GetHeight() / src.Rows };
				glm::vec2 spriteSize = { src.SubtextureWidth, src.SubtextureHeight };
				Ref<SubTexture2D> subtexture = SubTexture2D::CreateFromCoords(texture, coords, cellSize, spriteSize);
				DrawQuad(transform, subtexture, src.Color, src.TilingFactor, entityID);
			} else DrawQuad(transform, src.Color, entityID);
		} else if (src.Type == SpriteRendererComponent::SpriteType::ANIMATION) {
			if (texture) {
				if (src.CurrentFrame > src.EndFrame) src.CurrentFrame = src.StartFrame;
				else if (src.CurrentFrame < src.StartFrame) src.CurrentFrame = src.StartFrame;

				src.ColumnCoordinate = (src.CurrentFrame % src.Columns);
				if (src.ColumnCoordinate == 0) src.ColumnCoordinate = src.Columns;
				src.RowCoordinate = ((src.CurrentFrame - 1) / src.Columns) + 1;
				
				glm::vec2 coords = { src.ColumnCoordinate - 1, (src.Rows - 1) - (src.RowCoordinate - 1) };
				glm::vec2 cellSize = { texture->GetWidth() / src.Columns, texture->GetHeight() / src.Rows };
				Ref<SubTexture2D> subtexture = SubTexture2D::CreateFromCoords(texture, coords, cellSize);
				DrawQuad(transform, subtexture, src.Color, src.TilingFactor, entityID);

				// this assumes running game at 60 fps
				src.FrameCounter++;
				if (src.FrameCounter >= 60 / src.FPS) {
					src.CurrentFrame++;
					src.FrameCounter = 0;
				}
			} else DrawQuad(transform, src.Color, entityID);
		} else FL_CORE_ASSERT(false, "Sprite type not supported!");
	}

	Renderer2D::Statistics Renderer2D::GetStats() {
		return s_Data.Stats;
	}

	void Renderer2D::ResetStats() {
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}
}