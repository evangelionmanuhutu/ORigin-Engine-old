#pragma once
#include "VertexArray.h"
#include <glm\glm.hpp>

namespace Origin
{
	class RendererAPI
	{
	public:

		enum class API
		{
			None = 0,
			OpenGL,
			OpenGL_ES,
			DirectX,
			Metal,
			VULKAN,
		};

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void ClearColor(glm::vec4 color) = 0;
		virtual void ClearColor(float r, float g, float b, float a) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t IndexCount = 0) = 0;

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};

}