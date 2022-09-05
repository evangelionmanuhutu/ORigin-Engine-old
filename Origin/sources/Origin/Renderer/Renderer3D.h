#pragma once
#include "Origin\Renderer\VertexArray.h"
#include "Origin\Renderer\Buffer.h"
#include "Origin\Renderer\Shader.h"
#include "Origin\Renderer\Texture.h"

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

		static void BeginScene(const Camera& camera, glm::mat4& transform);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::mat4& transform, const glm::vec4 color);
		static void DrawPlane(const glm::mat4& transform, glm::vec2& tillingFactor);
		static void DrawSkybox();

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

