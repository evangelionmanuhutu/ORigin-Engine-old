#include "Editor.h"
#include "Origin/EntryPoint.h"
#include "panels/EditorTheme.h"

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Origin\Utils\PlatformUtils.h"

#include "Origin\Scene\SceneSerializer.h"

#include <ImGuizmo.h>

namespace Origin {

	void Editor::OnAttach()
  {
    EditorTheme::ApplyDarkPurple();

    FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = Framebuffer::Create(fbSpec);
    
    m_ActiveScene = std::make_shared<Scene>();
    m_EditorCamera = EditorCamera(30.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

    m_SceneHierarchyPanel.SetContext(m_ActiveScene);

  }

  void Editor::OnEvent(Event& e)
  {
    m_EditorCamera.OnEvent(e);
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseMovedEvent>(OGN_BIND_EVENT_FN(Editor::OnMouseMovedEvent));
    dispatcher.Dispatch<WindowResizeEvent>(OGN_BIND_EVENT_FN(Editor::OnWindowResize));
    dispatcher.Dispatch<KeyPressedEvent>(OGN_BIND_EVENT_FN(Editor::OnKeyPressed));
  }

  bool Editor::OnKeyPressed(KeyPressedEvent& e)
  {
    if (e.GetRepeatCount() > 0)
      return false;

    bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
    bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

    switch (e.GetKeyCode())
    {
      // File Operation
      case Key::S:
      {
        if (control && shift)
          SaveSceneAs();

        m_GizmosType = ImGuizmo::OPERATION::SCALE;

        break;
      }

      case Key::O:
      {
        if (control)
          OpenScene();
        break;
      }

      case Key::N:
      {
        if (control)
          NewScene();
        break;
      }

      case Key::T:
      {
        m_GizmosType = ImGuizmo::OPERATION::TRANSLATE;
        break;
      }

      case Key::R:
      {
        m_GizmosType = ImGuizmo::OPERATION::ROTATE;
        break;
      }
    }

    return true;
  }

  void Editor::OnUpdate(Timestep time)
  {
    ViewportRefresh();

    m_EditorCamera.OnUpdate(time);

    Renderer2D::ResetStats();
    m_Framebuffer->Bind();
    RenderCommand::Clear();
    RenderCommand::ClearColor(clearColor);

    m_ActiveScene->OnUpdateEditor(time, m_EditorCamera);

    m_Framebuffer->Unbind();
  }

  void Editor::OnGuiRender()
  {
    EditorPanel::BeginDockspace();
    VpGui();
    MenuBar();

    m_SceneHierarchyPanel.OnImGuiRender();
    EditorPanel::EndDockspace();
  }

  void Editor::NewScene()
  {
    m_ActiveScene = std::make_shared<Scene>();
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
  }

  void Editor::SaveSceneAs()
  {
    std::string filepath = FileDialogs::SaveFile("ORigin Scene (*.origin)\0*.origin\0");
    if (!filepath.empty())
    {
      SceneSerializer serializer(m_ActiveScene);
      serializer.Serialize(filepath);
    }
  }

  void Editor::OpenScene()
  {
    std::string filepath = FileDialogs::OpenFile("ORigin Scene (*.origin)\0*.origin\0");
    if (!filepath.empty())
    {
      m_ActiveScene = std::make_shared<Scene>();
      m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
      m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
      m_SceneHierarchyPanel.SetContext(m_ActiveScene);

      SceneSerializer serializer(m_ActiveScene);
      serializer.Deserialize(filepath);
    }
  }

  static bool guiDockingSpaceOpen = true;
  static bool guiMenuFullscreen = true;
  static bool guiMenuStyle = false;
  static bool guiRenderStatus = true;
  static bool guiDebugInfo = true;
  static bool guiImGuiDemoWindow = true;

  void Editor::MenuBar()
  {
    ImGuiStyle& style = ImGui::GetStyle();
    auto& window = Application::Get();
    ImGuiIO& io = ImGui::GetIO();

    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("New", "Ctrl + N"))
           NewScene();

        if (ImGui::MenuItem("Open", "Ctrl+O"))
          OpenScene();

        if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
          SaveSceneAs();

        if (ImGui::MenuItem("Exit")) window.Close();

        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Window"))
      {
        ImGui::MenuItem("Style Editor", NULL, &guiMenuStyle);
        ImGui::MenuItem("Render Status", NULL, &guiRenderStatus);
        ImGui::MenuItem("Debug Info", NULL, &guiDebugInfo);
        ImGui::MenuItem("Demo Window", NULL, &guiImGuiDemoWindow);
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
    if (guiRenderStatus)
    {
      ImGui::Begin("Render Status", &guiRenderStatus);
      auto stats = Renderer2D::GetStats();
      ImGui::Text("Renderer2D Status");
      ImGui::Text("Draw Calls: %d", stats.Draw_Calls);
      ImGui::Text("Quads: %d", stats.Quad_Count);
      ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
      ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
      ImGui::End();
    }

    if (guiMenuStyle)
    {
      ImGui::Begin("Style Editor", &guiMenuStyle);
      ImGui::ShowStyleEditor();
      ImGui::End();
    }

    if (guiDebugInfo)
    {
      ImGui::Begin("Debug Info", &guiDebugInfo);
      ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::Text("OpenGL Version : (%s)", glGetString(GL_VERSION));
      ImGui::Text("ImGui version : (%s)", IMGUI_VERSION);
      ImGui::Separator();
      ImGui::Text("Mouse Position (%d, %d)", mouseX, mouseY);
      ImGui::End();
    }

    if (guiImGuiDemoWindow)
    {
      ImGui::ShowDemoWindow(&guiImGuiDemoWindow);
    }

  }

  void Editor::VpGui()
  {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar
      | ImGuiWindowFlags_NoScrollWithMouse
      | ImGuiWindowFlags_NoCollapse;

    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;

    ImGui::SetNextWindowClass(&window_class);
    ImGui::Begin("Viewport", nullptr, window_flags);
   

    auto viewportOffset = ImGui::GetCursorPos();

    ImGuiIO& io = ImGui::GetIO();
    if (ImGui::IsWindowHovered())
    {
      io.WantCaptureMouse = false;
      m_EditorCamera.ViewportHovered = true;
    }
    else
    {
      io.WantCaptureMouse = true;
      m_EditorCamera.ViewportHovered = false;
    }

    if (ImGui::IsWindowFocused())
    {
      m_EditorCamera.ViewportActive = true;
    }
    else
    {
      io.WantCaptureMouse = true;
      m_EditorCamera.ViewportActive = false;
    }

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

    uint32_t viewportID = m_Framebuffer->GetColorAttachmentRendererID();
    ImGui::Image((void*)viewportID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

    auto windowSize = ImGui::GetWindowSize();

    ImVec2 minBound = ImGui::GetWindowPos();
    minBound.x += viewportOffset.x;
    minBound.y += viewportOffset.y;

    ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
    m_ViewportBounds[0] = { minBound.x, minBound.y };
    m_ViewportBounds[1] = { maxBound.x, maxBound.y };

    auto [mx, my] = ImGui::GetMousePos();
    mx -= m_ViewportBounds[0].x;
    my -= m_ViewportBounds[0].y;
    glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
    my = viewportSize.y - my;

    mouseX = (int)mx;
    mouseY = (int)my - 9;

    if (mouseX < 0) mouseX = 0;
    else if (mouseX > (int)viewportSize.x) mouseX = (int)viewportSize.x;
    if (mouseY < 0) mouseY = 0;
    else if (mouseY > (int)viewportSize.y) mouseY = (int)viewportSize.y;

    Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
    if (selectedEntity && m_GizmosType != -1)
    {
      ImGuizmo::SetOrthographic(false);
      ImGuizmo::SetDrawlist();
      float windowWidth = ImGui::GetWindowWidth();
      float windowHeight = ImGui::GetWindowHeight();

      ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

      // Runtime Camera from Entity
      /*auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
      const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
      const glm::mat4& cameraProjection = camera.GetProjection();
      glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());*/

      // Editor Camera
      const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
      glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

      auto& tc = selectedEntity.GetComponent<TransformComponent>();
      glm::mat4 transform = tc.GetTransform();
      glm::vec3 originalRotation = tc.Rotation;

      bool snap = Input::IsKeyPressed(Key::LeftShift);
      float snapValue = 0.5f;

      if (snap && m_GizmosType == ImGuizmo::OPERATION::ROTATE)
        snapValue = 45.0f;

      float snapValues[3] = { snapValue, snapValue, snapValue };

      ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
        (ImGuizmo::OPERATION)m_GizmosType, ImGuizmo::LOCAL, glm::value_ptr(transform), 0, snap ? snapValues : nullptr);

      if (ImGuizmo::IsUsing())
      {
        m_EditorCamera.MovementActive = false;
        glm::vec3 translation, rotation, scale;
        Math::DecomposeTransform(transform, translation, rotation, scale);

        tc.Translation = translation;
        glm::vec3 deltaRotation = rotation - tc.Rotation;
        tc.Rotation += deltaRotation;
        tc.Scale = scale;
      }
      else
      {
        m_EditorCamera.MovementActive = true;
      }

      if (Input::IsKeyPressed(Key::Escape)) m_GizmosType = -1;
    }
    else if(!selectedEntity) m_GizmosType = -1;

    ImGui::End();
    ImGui::PopStyleVar();
      
  }

  void Editor::ViewportRefresh()
  {
    if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
      m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
      (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
    {
      m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
      m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
      m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    }
  }

  bool Editor::OnWindowResize(WindowResizeEvent& e)
  {
    //OGN_CORE_TRACE("{0}, {1}", e.getWidth(), e.getHeight());
    return false;
  }

  bool Editor::OnMouseMovedEvent(MouseMovedEvent& e)
  {
    //OGN_CORE_TRACE("{0}, {1}", e.GetX(), e.GetY());
    return false;
  }

  bool Editor::OnMouseButtonEvent(MouseButtonEvent& e)
  {
    //OGN_CORE_TRACE("{0}", e.GetMouseButton());
    return false;
  }
}
