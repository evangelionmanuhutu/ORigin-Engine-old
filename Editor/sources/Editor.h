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
      m_CameraController.OnEvent(e);
    }

    bool OnWindowResize(WindowResizeEvent& e);
    bool OnMouseMovedEvent(MouseMovedEvent& e);

    void OnGuiRender() override;

  private:
    Entity m_Square;

    glm::vec4 color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    glm::vec2 position = glm::vec2(0);
    glm::vec2 scale = glm::vec2(1);

  private:
    ShaderLibrary m_ShaderLibrary;
    OrthoCameraController m_CameraController;
    std::shared_ptr<Framebuffer> m_Framebuffer;
    std::shared_ptr<Scene> m_ActiveScene;

    EditorViewport vp;
  };
}