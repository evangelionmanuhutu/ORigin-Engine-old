#pragma once
#include <Origin.h>

namespace Origin {

  class Editor : public Layer
  {
  public:
    Editor() : Layer("Editor") {}

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
    ShaderLibrary m_ShaderLibrary;
    OrthoCameraController m_CameraController;
    std::shared_ptr<Framebuffer> m_Framebuffer;
    std::shared_ptr<Texture2D> m_Texture, m_Sprites, m_Logo;
    std::shared_ptr<SubTexture2D> m_StairsTexture;
    std::shared_ptr<SubTexture2D> m_BarrelTexture;
    std::shared_ptr<SubTexture2D> m_TreeTexture;
  private:
  };
}