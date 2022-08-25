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
    //m_ActiveScene = std::make_shared<Scene>();
  }

  void Editor::OnUpdate(Timestep ts)
  {
    m_CameraController.OnUpdate(ts);

    vp.Refresh(m_Framebuffer, m_CameraController);

    Renderer2D::ResetStats();
    Renderer2D::BeginScene(m_CameraController.GetCamera());
    Renderer2D::DrawQuad(position, scale, color);
    Renderer2D::EndScene();
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
    //auto& squareColor = m_Square.GetComponent<SpriteRendererComponent>().Color;
    ImGui::DragFloat2("position", glm::value_ptr(position), 0.01f);
    ImGui::DragFloat2("scale", glm::value_ptr(scale), 0.01f, 0);
    ImGui::ColorEdit4("color", glm::value_ptr(color));
    ImGui::End();

    ImGui::Begin("Conten Drawer");
    ImGui::End();

    EditorPanel::EndDockspace();
  }
}
