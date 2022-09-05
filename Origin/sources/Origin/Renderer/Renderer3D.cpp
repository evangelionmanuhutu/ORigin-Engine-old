#include "pch.h"
#include "Renderer3D.h"
#include "Origin\Scene\Component\Camera.h"
#include "RenderCommand.h"

#include "Platform\OpenGL\OpenGL_Shader.h"

#include <glm\gtc\matrix_transform.hpp>

namespace Origin
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};

	struct Renderer3D_Data
	{
		// Renderer3D Stuff
		Renderer3D::Statistics Stats;

		// Quads
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVetexBufferPtr = nullptr;

		std::shared_ptr<VertexArray> vertexArray;
		std::shared_ptr<VertexBuffer> vertexBuffer;

		std::shared_ptr<Shader> quadShader;
		std::shared_ptr<Texture2D> WhiteTexture;
		std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;
		glm::vec4 QuadVertexPosition[24];
	};

	static Renderer3D_Data s_3D_Data;

	void Renderer3D::Init()
	{
		s_3D_Data.vertexArray = VertexArray::Create();

		s_3D_Data.vertexBuffer = VertexBuffer::Create(s_3D_Data.MaxVertices * sizeof(QuadVertex));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "aPos"},
			{ ShaderDataType::Float4, "aColor"},
		};

		s_3D_Data.vertexBuffer->SetLayout(layout);
		s_3D_Data.vertexArray->AddVertexBuffer(s_3D_Data.vertexBuffer);

		s_3D_Data.QuadVertexBufferBase = new QuadVertex[s_3D_Data.MaxVertices];

		uint32_t* QuadIndices = new uint32_t[s_3D_Data.MaxIndices];

		uint32_t Offset = 0;
		for (uint32_t i = 0; i < s_3D_Data.MaxIndices; i += 6)
		{
			QuadIndices[i + 0] = Offset + 0;
			QuadIndices[i + 1] = Offset + 1;
			QuadIndices[i + 2] = Offset + 2;

			QuadIndices[i + 3] = Offset + 2;
			QuadIndices[i + 4] = Offset + 3;
			QuadIndices[i + 5] = Offset + 0;

			Offset += 4;
		}

		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(QuadIndices, s_3D_Data.MaxIndices);
		s_3D_Data.vertexArray->SetIndexBuffer(indexBuffer);
		delete[] QuadIndices;

		s_3D_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_3D_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_3D_Data.TextureSlots[0] = s_3D_Data.WhiteTexture;

		int32_t samplers[s_3D_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_3D_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_3D_Data.quadShader = Shader::Create("assets/shaders/Default3DQuad.glsl");
		s_3D_Data.quadShader->Bind();
		s_3D_Data.quadShader->SetIntArray("u_Textures", samplers, s_3D_Data.MaxTextureSlots);

		// Front
		s_3D_Data.QuadVertexPosition[0]  = glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[1]  = glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[2]  = glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[3]  = glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f);
		// Back
		s_3D_Data.QuadVertexPosition[4]  = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[5]  = glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[6]  = glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[7]  = glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f);
		// Left
		s_3D_Data.QuadVertexPosition[8]  = glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[9]  = glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[10] = glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[11] = glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f);
		// Right
		s_3D_Data.QuadVertexPosition[12] = glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[13] = glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[14] = glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[15] = glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f);
		// Top
		s_3D_Data.QuadVertexPosition[16] = glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[17] = glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[18] = glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[19] = glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f);
		// Bottom
		s_3D_Data.QuadVertexPosition[20] = glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[21] = glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[22] = glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f);
		s_3D_Data.QuadVertexPosition[23] = glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f);
	}
	

	void Renderer3D::BeginScene(const Camera& camera, glm::mat4& transform)
	{
		glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);

		s_3D_Data.quadShader->Bind();
		s_3D_Data.quadShader->SetMatrix("u_ViewProjection", viewProjection);

		StartBatch();
	}

	void Renderer3D::EndScene()
	{
		Flush();
	}

	void Renderer3D::Flush()
	{
		if (s_3D_Data.QuadIndexCount == 0)
			return;
		// Quads
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_3D_Data.QuadVetexBufferPtr - (uint8_t*)s_3D_Data.QuadVertexBufferBase);
			s_3D_Data.vertexBuffer->SetData(s_3D_Data.QuadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_3D_Data.TextureSlotIndex; i++)
				s_3D_Data.TextureSlots[i]->Bind(i);

			RenderCommand::DrawTriIndexed(s_3D_Data.vertexArray, s_3D_Data.QuadIndexCount);
			s_3D_Data.Stats.Draw_Calls++; s_3D_Data.quadShader->Unbind();
		}
	}

	void Renderer3D::Shutdown()
	{
		delete[] s_3D_Data.QuadVertexBufferBase;
	}

	void Renderer3D::StartBatch()
	{
		s_3D_Data.QuadIndexCount = 0;
		s_3D_Data.QuadVetexBufferPtr = s_3D_Data.QuadVertexBufferBase;
		s_3D_Data.TextureSlotIndex = 1;
	}

	void Renderer3D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer3D::DrawQuad(const glm::mat4& transform, const glm::vec4 color)
	{
		constexpr int QuadVertexCount = 24;

		const float texIndex = 0.0f;
		const float texTiling = 1.0f;

		if (s_3D_Data.QuadIndexCount >= s_3D_Data.MaxIndices)
			Renderer3D::StartBatch();

		for (size_t i = 0; i < QuadVertexCount; i++)
		{
			s_3D_Data.QuadVetexBufferPtr->Position = transform * s_3D_Data.QuadVertexPosition[i];
			s_3D_Data.QuadVetexBufferPtr->Color = color;
			s_3D_Data.QuadVetexBufferPtr++;
		}
		s_3D_Data.QuadIndexCount += 6;
		s_3D_Data.Stats.Quad_Count++;
		
	}

	void Renderer3D::DrawPlane(const glm::mat4& transform, glm::vec2& tillingFactor)
	{
	}

	void Renderer3D::DrawSkybox()
	{
	}
}