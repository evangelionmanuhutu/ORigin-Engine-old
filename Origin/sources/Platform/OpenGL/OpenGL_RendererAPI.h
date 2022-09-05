#pragma once
#include "Origin\Renderer\RendererAPI.h"
#include <glm\glm.hpp>

namespace Origin
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void ClearColor(glm::vec4 color) override;
		virtual void ClearColor(float r, float g, float b, float a) override;

		virtual void Clear() override;

		virtual void DrawTriIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t IndexCount = 0) override;
		virtual void DrawTriArrays(const std::shared_ptr<VertexArray>& vertexArray, uint32_t Count = 0) override;

		virtual void DrawLineTriIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t IndexCount) override;
		virtual void DrawLineTriArrays(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count) override;
	};
}