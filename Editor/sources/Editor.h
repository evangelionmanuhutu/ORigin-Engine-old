#pragma once
#include <Origin.h>
#include "panels/EditorPanel.h"

namespace Origin {

  class Editor : public Layer
  {
  public:
    Editor() : Layer("Editor")
    {
    }

    void OnAttach() override;
    void OnUpdate(Timestep ts) override;

    void OnEvent(Event& e) override
  	{
      EventDispatcher dispatcher(e);
      dispatcher.Dispatch<MouseMovedEvent>(OGN_BIND_EVENT_FN(Editor::OnMouseMovedEvent));
      dispatcher.Dispatch<WindowResizeEvent>(OGN_BIND_EVENT_FN(Editor::OnWindowResize));
    }

    bool OnWindowResize(WindowResizeEvent& e);
    bool OnMouseMovedEvent(MouseMovedEvent& e);

    void OnGuiRender() override;

  private:
    void VpRefresh();
    void VpGui();

    Entity m_SquareEntity;
    Entity m_CameraEntity;
    Entity m_SecondCameraEntity;
    bool CamPrimary = true;

    glm::vec4 color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    glm::vec2 position = glm::vec2(0);
    glm::vec2 scale = glm::vec2(1);

    glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
    glm::vec2 m_ViewportBounds[2];

  private:
    ShaderLibrary m_ShaderLibrary;
    std::shared_ptr<Framebuffer> m_Framebuffer;
    std::shared_ptr<Scene> m_ActiveScene;
  };
}