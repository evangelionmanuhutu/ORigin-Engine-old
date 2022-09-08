#pragma once
#include <Origin.h>
#include "panels\EditorPanel.h"
#include "panels\SceneHierarchyPanel.h"

namespace Origin {

  class Editor : public Layer
  {
  public:
    Editor() : Layer("Editor")
    {
    }

    void OnAttach() override;
    void OnUpdate(Timestep ts) override;

    void OnEvent(Event& e) override;

    void ViewportRefresh();

    bool OnWindowResize(WindowResizeEvent& e);
    bool OnMouseMovedEvent(MouseMovedEvent& e);
    bool OnMouseButtonEvent(MouseButtonEvent& e);
    bool OnKeyPressed(KeyPressedEvent& e);

    void OnGuiRender() override;

  private:
    void NewScene();
    void SaveSceneAs();
    void OpenScene();

    void VpGui();
    void MenuBar();

    Entity m_SquareEntity;
    Entity m_SquareEntity2;
    Entity m_Camera;
    Entity m_Camera2;

    glm::vec4 clearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

    glm::vec4 color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    glm::vec2 position = glm::vec2(0);
    glm::vec2 scale = glm::vec2(1);

    glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
    glm::vec2 m_ViewportBounds[2];

  private:
    ShaderLibrary m_ShaderLibrary;
    std::shared_ptr<Framebuffer> m_Framebuffer;
    std::shared_ptr<Scene> m_ActiveScene;
    SceneHierarchyPanel m_SceneHierarchyPanel;

    int mouseX;
    int mouseY;
  };

}