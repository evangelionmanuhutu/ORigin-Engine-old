#include "pch.h"
#include "Renderer3D.h"

namespace Origin
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec3 TexCoords;
		glm::vec4 Color;
		float TexIndex;
		float TexTiling;
	};

	struct CubemapVertex
	{
		glm::vec3 Position;
		glm::vec3 TexCoords;
		glm::vec4 ColorTint;
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
		glm::vec4 QuadVertexPosition[4];
	};

	void Renderer3D::Init()
	{

	}
}