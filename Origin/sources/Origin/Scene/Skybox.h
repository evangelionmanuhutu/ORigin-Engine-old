#pragma once
#include <stb_image.h>
#include "Origin\Renderer\Shader.h"
#include "Origin\Renderer\VertexArray.h"
#include "Origin\Renderer\Buffer.h"
#include "Origin\Scene\Component\Camera.h"

namespace Origin
{
	class Skybox
	{
	public:
		static void Init();
		static void LoadTexture();
		static void Draw(const Camera& camera, glm::mat4& transform);
	};

}

