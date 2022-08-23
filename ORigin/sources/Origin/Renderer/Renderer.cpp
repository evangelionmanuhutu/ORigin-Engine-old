#include "pch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Origin\Renderer\Renderer2D.h"
#include "Platform\OpenGL\OpenGL_Shader.h"

namespace Origin
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::OnUpdate()
	{
		RenderCommand::ClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();
	}

	void Renderer::BeginScene(OrthoCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetMatrix("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetMatrix("u_Transform", transform);

		vertexArray->Bind();
		vertexArray->GetIndexBuffer()->Bind();

		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Shutdown()
	{
		OGN_CORE_TRACE("Renderer::Shutdown");
		delete m_SceneData;
		Renderer2D::Shutdown();
	}

}