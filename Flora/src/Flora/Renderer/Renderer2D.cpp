#include "flpch.h"
#include "Renderer2D.h"
#include "Flora/Renderer/VertexArray.h"
#include "Flora/Renderer/Shader.h"
#include "Flora/Renderer/RenderCommand.h"
#include "Flora/Renderer/StaticShaders.h"
#ifdef WIN_BUILD_ONLY
#include "Flora/Core/HiddenStructs.h"
#endif
#include "Flora/Math/Math.h"
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

	struct CircleVertex {
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		// Editor-only
		int EntityID;
	};

	struct LineVertex {
		glm::vec3 Position;
		glm::vec4 Color;

		// Editor-only
		int EntityID;
	};

	struct TextVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;

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

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		Ref<VertexArray> TextVertexArray;
		Ref<VertexBuffer> TextVertexBuffer;
		Ref<Shader> TextShader;

		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		uint32_t TextIndexCount = 0;
		TextVertex* TextVertexBufferBase = nullptr;
		TextVertex* TextVertexBufferPtr = nullptr;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;
		float LineWidth = 2.5f;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 is the white texture

		Ref<Texture2D> FontAtlasTexture;

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

		s_Data.WhiteTexture = Texture2D::Create(TextureSpecification());
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}

		s_Data.TextureShader = Shader::Create(StaticShaders::TextureGLSL(), "Texture");
		s_Data.CircleShader = Shader::Create(StaticShaders::CircleGLSL(), "Circle");
		s_Data.LineShader = Shader::Create(StaticShaders::LineGLSL(), "Line");
		s_Data.TextShader = Shader::Create(StaticShaders::TextGLSL(), "Text");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		// circles
		s_Data.CircleVertexArray = VertexArray::Create();

		s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));
		s_Data.CircleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_WorldPosition" },
			{ ShaderDataType::Float3, "a_LocalPosition" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float, "a_Thickness" },
			{ ShaderDataType::Float, "a_Fade" },
			{ ShaderDataType::Int, "a_EntityID" }
		});
		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
		s_Data.CircleVertexArray->SetIndexBuffer(quadIB); // using quad ib on purpose
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];

		// lines
		s_Data.LineVertexArray = VertexArray::Create();

		s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));
		s_Data.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Int, "a_EntityID" }
		});
		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

		// text
		s_Data.TextVertexArray = VertexArray::Create();

		s_Data.TextVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(TextVertex));
		s_Data.TextVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Int, "a_EntityID" }
		});
		s_Data.TextVertexArray->AddVertexBuffer(s_Data.TextVertexBuffer);
		s_Data.TextVertexArray->SetIndexBuffer(quadIB); // using quad ib on purpose
		s_Data.TextVertexBufferBase = new TextVertex[s_Data.MaxVertices];

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown() {
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const glm::mat4 viewProjection) {
		FL_PROFILE_FUNCTION();
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", viewProjection);

		s_Data.CircleShader->Bind();
		s_Data.CircleShader->SetMat4("u_ViewProjection", viewProjection);

		s_Data.TextShader->Bind();
		s_Data.TextShader->SetMat4("u_ViewProjection", viewProjection);

		s_Data.LineShader->Bind();
		s_Data.LineShader->SetMat4("u_ViewProjection", viewProjection);

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) {
		FL_PROFILE_FUNCTION();
		glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);
		BeginScene(viewProj);
	}

	void Renderer2D::EndScene() {
		FL_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::StartBatch() {
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.TextIndexCount = 0;
		s_Data.TextVertexBufferPtr = s_Data.TextVertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;
	}

	void Renderer2D::Flush() {
		bool drawCall = false;
		if (s_Data.QuadIndexCount) {
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);
			s_Data.TextureShader->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
			drawCall = true;
		}

		if (s_Data.CircleIndexCount) {
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

			s_Data.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
			drawCall = true;
		}

		if (s_Data.TextIndexCount) {
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.TextVertexBufferPtr - (uint8_t*)s_Data.TextVertexBufferBase);
			s_Data.TextVertexBuffer->SetData(s_Data.TextVertexBufferBase, dataSize);

			// Bind textures
			s_Data.FontAtlasTexture->Bind(0);
			s_Data.TextShader->Bind();
			RenderCommand::DrawIndexed(s_Data.TextVertexArray, s_Data.TextIndexCount);
			drawCall = true;
		}

		if (s_Data.LineVertexCount) {
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

			s_Data.LineShader->Bind();
			RenderCommand::SetLineThickness(s_Data.LineWidth);
			RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
			drawCall = true;
		}

		if (drawCall) s_Data.Stats.DrawCalls++;
	}

	void Renderer2D::NextBatch() {
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID) {
		FL_PROFILE_FUNCTION();

		if (s_Data.CircleIndexCount >= Renderer2DData::MaxIndices) NextBatch();

		for (size_t i = 0; i < 4; i++) {
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;
			s_Data.CircleVertexBufferPtr->Fade = fade;
			s_Data.CircleVertexBufferPtr->EntityID = entityID;
			s_Data.CircleVertexBufferPtr++;
		}

		s_Data.CircleIndexCount += 6;
		s_Data.Stats.CircleCount++;
	}

	void Renderer2D::DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color, int entityID) {
		s_Data.LineVertexBufferPtr->Position = p1;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = p2;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;
		s_Data.Stats.LineCount++;
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
				if (s_Data.TextureSlots[i] == texture) {
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
				if (s_Data.TextureSlots[i] == texture) {
					texture;
					s_Data.TextureSlots[i];
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
				if (s_Data.TextureSlots[i] == texture) {
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

	void Renderer2D::DrawSprite(Timestep ts, const glm::mat4& transform, SpriteRendererComponent& src, int entityID) {
		Ref<Texture2D> texture = nullptr;
		if (src.Path != "NULL") {
			if (!src.TextureInitialized) {
				AssetManager::AddTexture(src.Path);
				src.TextureInitialized = true;
			}
			texture = AssetManager::GetTexture(src.Path);
		}
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

				if (!src.Paused) {
					src.Time += ts;
					int numFrames = src.EndFrame - src.StartFrame + 1;
					float frametime = 1.0f / ((float)src.FPS);
					float maxtime = frametime * numFrames;
					if (src.Time >= maxtime) src.Time = 0.0f;
					src.CurrentFrame = src.StartFrame + (int)(src.Time / frametime);
					src.FrameCounter = src.CurrentFrame - src.StartFrame;
				}
			} else DrawQuad(transform, src.Color, entityID);
		} else FL_CORE_ASSERT(false, "Sprite type not supported!");
	}

	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float rotation, int entityID) {
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
		DrawRect(transform, color, entityID);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID) {
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * s_Data.QuadVertexPositions[i];

		DrawLine(lineVertices[0], lineVertices[1], color);
		DrawLine(lineVertices[1], lineVertices[2], color);
		DrawLine(lineVertices[2], lineVertices[3], color);
		DrawLine(lineVertices[3], lineVertices[0], color);
	}

	#ifdef WIN_BUILD_ONLY
	void Renderer2D::DrawString(TextComponent& tc, Entity entity) {
		if (!tc.FontInitialized) {
			AssetManager::AddFont(tc.FontFilePath);
			tc.FontInitialized = true;
		}
		TextConfig conf;
		TransformComponent tfc = entity.GetComponent<TransformComponent>();
		conf.TextString = tc.TextString;
		conf.Kerning = tc.Kerning;
		conf.LineSpacing = tc.LineSpacing;
		conf.Color = tc.Color;
		conf.Translation = tfc.Translation + tc.Translation;
		conf.Rotation = tfc.Rotation + tc.Rotation;
		conf.Scale = { tfc.Scale.x * tc.Scale.x, tfc.Scale.y * tc.Scale.y, tfc.Scale.z * tc.Scale.z };
		conf.Alignment = tc.Alignment;
		conf.EntityID = (int)(uint32_t)entity;
		conf.Font = AssetManager::GetFont(tc.FontFilePath);
		DrawString(conf);
	}

	void Renderer2D::DrawString(const TextConfig& config) {
		Ref<Font> font = config.Font ? config.Font : Font::GetDefault();
		const auto& geometry = font->GetFontData()->Geometry;
		const auto& metrics = geometry.getMetrics();
		Ref<Texture2D> fontAtlas = font->GetAtlasTexture();
		s_Data.FontAtlasTexture = fontAtlas;

		double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
		float kerningOffset = config.Kerning;
		float lineSpacing = config.LineSpacing;
		auto spaceGlyph = geometry.getGlyph(' ');
		int numlines = 1;
		for (size_t i = 0; i < config.TextString.size(); i++)
			if (config.TextString[i] == '\n') numlines++;

		std::vector<double> startpos(numlines, 0);
		int startposind = 0;
		if (config.Alignment != FontAlignment::LEFT) {
			int lineind = 0;
			double advance;
			for (size_t i = 0; i < config.TextString.size(); i++) {
				if (config.TextString[i] == '\n') {
					lineind++;
					continue;
				}
				if (config.TextString[i] == '\r') continue;
				if (config.TextString[i] == '\t') {
					advance = spaceGlyph->getAdvance();
					startpos[lineind] -= 4 * (fsScale * advance + kerningOffset);
				}
				else if (config.TextString[i] == ' ' && i < config.TextString.size() - 1) {
					geometry.getAdvance(advance, config.TextString[i], config.TextString[i + 1]);
					startpos[lineind] -= fsScale * advance + kerningOffset;
				}
				else if (i < config.TextString.size() - 1) {
					auto glyph = geometry.getGlyph(config.TextString[i]);
					if (!glyph) glyph = geometry.getGlyph('?');
					if (!glyph) return;
					advance = glyph->getAdvance();
					char nextCharacter = config.TextString[i + 1];
					geometry.getAdvance(advance, config.TextString[i], nextCharacter);
					startpos[lineind] -= fsScale * advance + kerningOffset;
				}
			}
			if (config.Alignment == FontAlignment::MIDDLE) {
				for (auto& pos : startpos)
					pos += ((-1.0 * pos) / 2.0);
			}
		}

		double x = startpos[startposind];
		double y = 0.0;

		for (size_t i = 0; i < config.TextString.size(); i++) {
			char character = config.TextString[i];
			if (character == '\r') continue;
			if (character == '\n') {
				startposind++;
				x = startpos[startposind];
				y -= fsScale * metrics.lineHeight + lineSpacing;
				continue;
			}
			if (character == ' ' && i < config.TextString.size() - 1) {
				double advance;
				char nextCharacter = config.TextString[i + 1];
				geometry.getAdvance(advance, character, nextCharacter);
				x += fsScale * advance + kerningOffset;
				continue;
			}
			if (character == '\t') {
				double advance = spaceGlyph->getAdvance();
				x += 4*(fsScale * advance + kerningOffset);
				continue;
			}
			auto glyph = geometry.getGlyph(character);
			if (!glyph) glyph = geometry.getGlyph('?');
			if (!glyph) return;

			double al, ab, ar, at;
			glyph->getQuadAtlasBounds(al, ab, ar, at);
			glm::vec2 texCoordMin((float)al, (float)ab);
			glm::vec2 texCoordMax((float)ar, (float)at);

			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);
			glm::vec2 quadMin((float)pl, (float)pb);
			glm::vec2 quadMax((float)pr, (float)pt);

			quadMin *= fsScale, quadMax *= fsScale;
			quadMin += glm::vec2(x, y);
			quadMax += glm::vec2(x, y);

			float texelWidth = 1.0f / fontAtlas->GetWidth();
			float texelHeight = 1.0f / fontAtlas->GetHeight();
			texCoordMin *= glm::vec2(texelWidth, texelHeight);
			texCoordMax *= glm::vec2(texelWidth, texelHeight);

			// rendering time
			glm::mat4 transform = Math::ComposeTransform(config.Translation, config.Rotation, config.Scale);

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = config.Color;
			s_Data.TextVertexBufferPtr->TexCoord = texCoordMin;
			s_Data.TextVertexBufferPtr->EntityID = config.EntityID;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = config.Color;
			s_Data.TextVertexBufferPtr->TexCoord = { texCoordMin.x, texCoordMax.y };
			s_Data.TextVertexBufferPtr->EntityID = config.EntityID;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = config.Color;
			s_Data.TextVertexBufferPtr->TexCoord = texCoordMax;
			s_Data.TextVertexBufferPtr->EntityID = config.EntityID;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = config.Color;
			s_Data.TextVertexBufferPtr->TexCoord = { texCoordMax.x, texCoordMin.y };
			s_Data.TextVertexBufferPtr->EntityID = config.EntityID;
			s_Data.TextVertexBufferPtr++;

			s_Data.TextIndexCount += 6;
			s_Data.Stats.QuadCount++;

			if (i < config.TextString.size() - 1) {
				double advance = glyph->getAdvance();
				char nextCharacter = config.TextString[i + 1];
				geometry.getAdvance(advance, character, nextCharacter);

				x += fsScale * advance + kerningOffset;
			}
		}
	}
	#endif

	Renderer2D::Statistics Renderer2D::GetStats() {
		return s_Data.Stats;
	}

	void Renderer2D::ResetStats() {
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	float Renderer2D::GetLineWidth() {
		return s_Data.LineWidth;
	}

	void Renderer2D::SetLineWidth(float width) {
		s_Data.LineWidth = width;
	}
}