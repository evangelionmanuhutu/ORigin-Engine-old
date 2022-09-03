#pragma once
#include "Origin\Renderer\VertexArray.h"
#include "Origin\Renderer\Buffer.h"
#include "Origin\Renderer\Shader.h"
#include "Origin\Renderer\Texture.h"
#include "Origin\Renderer\SubTexture2D.h"

#include "Origin\Scene\Component\Camera.h"

#include "glm\glm.hpp"

namespace Origin
{
	class Renderer3D
	{
	private:

	public:
		static void Init();
		static void Shutdown();

		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec3& position = glm::vec3(0.0f), const glm::vec2& scale = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

		struct Statistics
		{
			uint32_t Draw_Calls = 0;
			uint32_t Quad_Count = 0;

			uint32_t GetTotalVertexCount() { return Quad_Count * 4; }
			uint32_t GetTotalIndexCount() { return Quad_Count * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();

	private:
		static void StartBatch();
		static void NextBatch();
	};
}

