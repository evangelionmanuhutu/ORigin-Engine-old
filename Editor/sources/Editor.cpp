#include "Editor.h"
#include "Origin/EntryPoint.h"
#include <glm/gtc/type_ptr.hpp>

#include "panels/EditorTheme.h"
#include "panels/EditorPanel.h"

namespace Origin {

	void Editor::OnAttach()
  {
    EditorTheme::Apply();
    FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = Framebuffer::Create(fbSpec);
  }

  void Editor::OnUpdate(Timestep ts)
  {
    EditorPanel::BeginViewport(m_Framebuffer, m_CameraController);
    RenderCommand::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    RenderCommand::Clear();

    m_CameraController.OnUpdate(ts);
    Renderer2D::ResetStats();
    Renderer2D::BeginScene(m_CameraController.GetCamera());
    Renderer2D::DrawQuad(glm::vec3(0.0f), glm::vec2(0.5f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    Renderer2D::EndScene();
    EditorPanel::EndViewport(m_Framebuffer);
  }

  bool Editor::OnWindowResize(WindowResizeEvent& e)
  {
    return false;
  }

  bool Editor::OnMouseMovedEvent(MouseMovedEvent& e)
  {

    return false;
  }

  void Editor::OnGuiRender()
  {
    EditorPanel::BeginDockspace();

    EditorPanel::Viewport(m_Framebuffer, m_CameraController);
    EditorPanel::MenuBar();

    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("Conten Drawer");
    ImGui::End();

    EditorPanel::EndDockspace();
  }
}
