#include "Editor.h"
#include "Origin/EntryPoint.h"
#include <glm/gtc/type_ptr.hpp>

#include "panels/EditorTheme.h"

namespace Origin {

	void Editor::OnAttach()
  {
    m_CameraController.EditorMode(false);
    EditorTheme::Apply();
    FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = Framebuffer::Create(fbSpec);
  }

  void Editor::OnUpdate(Timestep ts)
  {

    float mouseX = (float)vp.GetMouseX();
    float mouseY = (float)vp.GetMouseY();

    mouseX = (mouseX / vp.GetWidth()) * 2 - 1;
    mouseY = (mouseY / vp.GetHeight()) * 2 - 1;
    mouseX = mouseX * m_CameraController.GetZoomLevel();
    mouseY = mouseY * m_CameraController.GetZoomLevel();

    vp.Refresh(m_Framebuffer, m_CameraController);
    RenderCommand::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    RenderCommand::Clear();

    m_CameraController.OnUpdate(ts);
    Renderer2D::ResetStats();
    Renderer2D::BeginScene(m_CameraController.GetCamera());
    Renderer2D::DrawQuad(glm::vec3(mouseX, mouseY, 0.0f), glm::vec2(0.3f), glm::vec4(color));
    Renderer2D::DrawQuad(glm::vec3(1.0f, 0.0f, -0.1f), glm::vec2(0.5f), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
    Renderer2D::EndScene();

    OGN_CORE_INFO("x {0} y {1}", mouseX, mouseY);
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
    EditorPanel::MenuBar();
    vp.GuiViewport(m_Framebuffer);

    ImGui::Begin("Quad");
    ImGui::ColorEdit4("color", glm::value_ptr(color));
    ImGui::End();

    ImGui::Begin("Conten Drawer");
    ImGui::End();

    EditorPanel::EndDockspace();
  }
}
