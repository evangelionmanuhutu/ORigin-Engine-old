#pragma once
#include "RendererAPI.h"

namespace Origin
{
	class RenderCommand
	{
	public:

		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void ClearColor(glm::vec4 color)
		{
			s_RendererAPI->ClearColor(color);
		}

		inline static void ClearColor(float r, float g, float b, float a)
		{
			s_RendererAPI->ClearColor(r, g, b, a);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawTriIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t IndexCount = 0)
		{
			s_RendererAPI->DrawTriIndexed(vertexArray, IndexCount);
		}

		inline static void DrawTriArrays(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
		{
			s_RendererAPI->DrawTriArrays(vertexArray, count);
		}

		inline static void DrawLineTriIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t IndexCount = 0)
		{
			s_RendererAPI->DrawLineTriIndexed(vertexArray, IndexCount);
		}

		inline static void DrawLineTriArrays(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
		{
			s_RendererAPI->DrawLineTriArrays(vertexArray, count);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};

}

