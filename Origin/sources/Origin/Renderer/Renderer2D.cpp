#include "pch.h"
#include "Renderer2D.h"
#include "Origin\Scene\Component\Camera.h"
#include "RenderCommand.h"

#include "Platform\OpenGL\OpenGL_Shader.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Origin
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoords;
		glm::vec4 Color;
		float TexIndex;
		float TexTiling;
	};

	struct Renderer2D_Data
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		std::shared_ptr<VertexArray> vertexArray;
		std::shared_ptr<VertexBuffer> vertexBuffer;

		std::shared_ptr<Shader> QuadShader;
		std::shared_ptr<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVetexBufferPtr = nullptr;

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 QuadVertexPosition[4];
		Renderer2D::Statistics Stats;
	};

	static Renderer2D_Data s_Data;

	void Renderer2D::Init()
	{
		s_Data.vertexArray = VertexArray::Create();

		s_Data.vertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "aPos"},
			{ ShaderDataType::Float2, "aTexCoords"},
			{ ShaderDataType::Float4, "aColor"},
			{ ShaderDataType::Float, "aTexID"},
			{ ShaderDataType::Float, "aTexTiling"},
		};

		s_Data.vertexBuffer->SetLayout(layout);
		s_Data.vertexArray->AddVertexBuffer(s_Data.vertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* QuadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t Offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			QuadIndices[i + 0] = Offset + 0;
			QuadIndices[i + 1] = Offset + 1;
			QuadIndices[i + 2] = Offset + 2;

			QuadIndices[i + 3] = Offset + 2;
			QuadIndices[i + 4] = Offset + 3;
			QuadIndices[i + 5] = Offset + 0;

			Offset += 4;
		}

		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(QuadIndices, s_Data.MaxIndices);
		s_Data.vertexArray->SetIndexBuffer(indexBuffer);
		delete[] QuadIndices;

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.QuadShader = Shader::Create("assets/shaders/Default2D.glsl");
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.QuadVertexPosition[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexPosition[1] = glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexPosition[2] = glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f);
		s_Data.QuadVertexPosition[3] = glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f);
	}

	void Renderer2D::BeginScene(const OrthoCamera& camera)
	{
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMatrix("u_ViewProjection", camera.GetViewProjectionMatrix());

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, glm::mat4& transform)
	{
		glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMatrix("u_ViewProjection", viewProjection);

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMatrix("u_ViewProjection", camera.GetViewProjection());

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		Flush();
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVetexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::Flush()
	{
		if (s_Data.QuadIndexCount == 0)
			return;

		// Quads
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVetexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.vertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);

			RenderCommand::DrawTriIndexed(s_Data.vertexArray, s_Data.QuadIndexCount);
			s_Data.Stats.Draw_Calls++; s_Data.QuadShader->Unbind();
		}
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data.QuadVertexBufferBase;
	}




	// PRIMITIVES
	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4 color)
	{
		constexpr int QuadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		const float texIndex = 0.0f;
		const float texTiling = 1.0f;

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			Renderer2D::StartBatch();

		for (size_t i = 0; i < QuadVertexCount; i++)
		{
			s_Data.QuadVetexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVetexBufferPtr->Color = color;
			s_Data.QuadVetexBufferPtr->TexCoords = textureCoords[i];
			s_Data.QuadVetexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVetexBufferPtr->TexTiling = texTiling;
			s_Data.QuadVetexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;
		s_Data.Stats.Quad_Count++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, float textureTiling, const glm::vec4& tintColor)
	{
		constexpr int QuadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
				Renderer2D::StartBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < QuadVertexCount; i++)
		{
			s_Data.QuadVetexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVetexBufferPtr->Color = tintColor;
			s_Data.QuadVetexBufferPtr->TexCoords = textureCoords[i];
			s_Data.QuadVetexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVetexBufferPtr->TexTiling = textureTiling;
			s_Data.QuadVetexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;
		s_Data.Stats.Quad_Count++;
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position))
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), scale, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const std::shared_ptr<Texture2D>& texture, float textureTiling, const glm::vec4& tintColor)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position))
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));
		DrawQuad(transform, texture, textureTiling, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, const std::shared_ptr<Texture2D>& texture, float textureTiling, const glm::vec4& tintColor)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), scale, texture, textureTiling, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale,
		const std::shared_ptr<SubTexture2D>& subTexture, float textureTiling, const glm::vec4& tintColor)
	{
		constexpr int QuadVertexCount = 4;
		const glm::vec2* textureCoords = subTexture->GetTexCoords();
		const std::shared_ptr<Texture2D> texture = subTexture->GetTexture();


		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
				Renderer2D::StartBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position))
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));

		for (size_t i = 0; i < QuadVertexCount; i++)
		{
			s_Data.QuadVetexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVetexBufferPtr->Color = tintColor;
			s_Data.QuadVetexBufferPtr->TexCoords = textureCoords[i];
			s_Data.QuadVetexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVetexBufferPtr->TexTiling = textureTiling;
			s_Data.QuadVetexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;
		s_Data.Stats.Quad_Count++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale,
		const std::shared_ptr<SubTexture2D>& subTexture, float textureTiling, const glm::vec4& tintColor)
	{
		DrawQuad(glm::vec3(position.x, position.y, 0.0f), scale, subTexture, textureTiling, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const glm::vec4& color)
	{
		constexpr int QuadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		const float texIndex = 0.0f;
		const float texTiling = 1.0f;

		if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
			Renderer2D::StartBatch();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position))
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1))
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));

		for (size_t i = 0; i < QuadVertexCount; i++)
		{
			s_Data.QuadVetexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVetexBufferPtr->Color = color;
			s_Data.QuadVetexBufferPtr->TexCoords = textureCoords[i];
			s_Data.QuadVetexBufferPtr->TexIndex = texIndex;
			s_Data.QuadVetexBufferPtr->TexTiling = texTiling;
			s_Data.QuadVetexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;
		s_Data.Stats.Quad_Count++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color)
	{
		DrawRotatedQuad(glm::vec3(position.x, position.y, 0.0f), rotation, scale, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const std::shared_ptr<Texture2D>& texture, float textureTiling, const glm::vec4& tintColor)
	{
		constexpr int QuadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
				Renderer2D::StartBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position))
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1))
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));

		for (size_t i = 0; i < QuadVertexCount; i++)
		{
			s_Data.QuadVetexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVetexBufferPtr->Color = tintColor;
			s_Data.QuadVetexBufferPtr->TexCoords = textureCoords[i];
			s_Data.QuadVetexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVetexBufferPtr->TexTiling = textureTiling;
			s_Data.QuadVetexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;
		s_Data.Stats.Quad_Count++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const std::shared_ptr<Texture2D>& texture, float textureTiling, const glm::vec4& tintColor)
	{
		DrawRotatedQuad(glm::vec3(position.x, position.y, 0.0f), rotation, scale, texture, textureTiling, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const std::shared_ptr<SubTexture2D>& subTexture, float textureTiling, const glm::vec4& tintColor)
	{
		constexpr int QuadVertexCount = 4;
		const glm::vec2* textureCoords = subTexture->GetTexCoords();
		const std::shared_ptr<Texture2D> texture = subTexture->GetTexture();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
				Renderer2D::StartBatch();

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position))
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1))
			* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, 1.0f));

		for (size_t i = 0; i < QuadVertexCount; i++)
		{
			s_Data.QuadVetexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVetexBufferPtr->Color = tintColor;
			s_Data.QuadVetexBufferPtr->TexCoords = textureCoords[i];
			s_Data.QuadVetexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVetexBufferPtr->TexTiling = textureTiling;
			s_Data.QuadVetexBufferPtr++;
		}
		s_Data.QuadIndexCount += 6;
		s_Data.Stats.Quad_Count++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const std::shared_ptr<SubTexture2D>& subTexture, float textureTiling, const glm::vec4& tintColor)
	{
		DrawRotatedQuad(glm::vec3(position.x, position.y, 0.0f), rotation, scale, subTexture, textureTiling, tintColor);
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	

	
}
