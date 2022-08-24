#include "pch.h"
#include "OpenGL_RendererAPI.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>

namespace Origin
{
	void OpenGLRendererAPI::Init()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			OGN_CORE_ERROR("FAILED TO INITIALIZE GLAD");
			glfwTerminate();
		}

		OGN_CORE_INFO("OpenGL Version: {0}", glGetString(GL_VERSION));
		OGN_CORE_INFO("OpenGL Vendor: {0}", glGetString(GL_VENDOR));
		OGN_CORE_INFO("OpenGL Renderer: {0}", glGetString(GL_RENDERER));

		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::ClearColor(glm::vec4 color)
	{
		glClearColor(color.r * color.a, color.g * color.a, color.b * color.a, color.a);
	}

	void OpenGLRendererAPI::ClearColor(float r, float g, float b, float a)
	{
		glClearColor(r * a, g * a, b * a, a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t IndexCount)
	{
		uint32_t count = IndexCount ? IndexCount : vertexArray->GetIndexBuffer()->GetCount();

		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}