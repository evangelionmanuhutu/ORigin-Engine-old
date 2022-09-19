#pragma once
#include "Origin\Renderer\VertexArray.h"
#include "Origin\Renderer\Buffer.h"
#include "Origin\Renderer\Shader.h"
#include "Origin\Renderer\Texture.h"

#include "Origin\Scene\Component\Camera.h"
#include "Origin\Scene\EditorCamera.h"

#include "glm\glm.hpp"

namespace Origin
{
	class Renderer3D
	{
	private:
		static void SkyboxInit();

	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera, glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawSkybox();

	private:
		
		static void StartBatch();
		static void NextBatch();
	};
}

