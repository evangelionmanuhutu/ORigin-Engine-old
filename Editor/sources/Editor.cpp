#include "Editor.h"
#include "Origin/EntryPoint.h"
#include "panels/EditorTheme.h"

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Origin\Utils\PlatformUtils.h"

#include "Origin\Scene\SceneSerializer.h"

namespace Origin {

	void Editor::OnAttach()
  {
    FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = Framebuffer::Create(fbSpec);
    m_ActiveScene = std::make_shared<Scene>();

#if 0
    m_SquareEntity = m_ActiveScene->CreateEntity("Entity 1");
    m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

    m_SquareEntity2 = m_ActiveScene->CreateEntity("Entity 2");
    m_SquareEntity2.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

    m_Camera = m_ActiveScene->CreateEntity("Camera A");
    m_Camera.AddComponent<CameraComponent>();
    auto& transform = m_Camera.GetComponent<TransformComponent>();
    transform.Translation.z = 8.0f;

    class EntityController : public ScriptableEntity
    {
      void OnCreate()
      {
        auto& transform = GetComponent<TransformComponent>();
        transform.Translation.x = 2.0f;
      }

      void OnUpdate(Timestep time)
      {
      }
    };

    m_SquareEntity2.AddComponent<NativeScriptComponent>().Bind<EntityController>();


    class CameraController : public ScriptableEntity
    {
    public:

      void OnCreate()
      {
        auto& translation = GetComponent<TransformComponent>().Translation;
        translation.z = 8.0f;
      }

      void OnUpdate(Timestep time)
      {
        auto& Translation = GetComponent<TransformComponent>().Translation;

        float speed = 5.0f;
        if (Input::IsKeyPressed(OGN_KEY_LEFT_SHIFT)) 
          speed = 10.0f;

        if (Input::IsKeyPressed(OGN_KEY_A)) 
          Translation.x -= speed * time;
        else if (Input::IsKeyPressed(OGN_KEY_D))
          Translation.x += speed * time;
        if (Input::IsKeyPressed(OGN_KEY_W))
          Translation.y += speed * time;
        else if (Input::IsKeyPressed(OGN_KEY_S))
          Translation.y -= speed * time;
      }
    };

    m_Camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
  }

  void Editor::OnEvent(Event& e)
  {
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
      case Key::S:
      {
        if (control && shift)
          SaveAs();
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
    }
    return false;
  }

  void Editor::OnUpdate(Timestep ts)
  {
    Renderer2D::ResetStats();
    m_Framebuffer->Bind();
    RenderCommand::Clear();
    RenderCommand::ClearColor(clearColor);

    ViewportRefresh();

    m_ActiveScene->OnUpdate(ts);
    m_Framebuffer->Unbind();
  }

  void Editor::OnGuiRender()
  {
    EditorPanel::BeginDockspace();
    VpGui();
    MenuBar();

    m_SceneHierarchyPanel.OnImGuiRender();

    ImGui::Begin("Debug Info");
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("OpenGL Version : (%s)", glGetString(GL_VERSION));
    ImGui::Text("ImGui version : (%s)", IMGUI_VERSION);
    ImGui::Separator();
    ImGui::Text("Mouse Position (%d, %d)", mouseX, mouseY);
    ImGui::ColorEdit4("Background", glm::value_ptr(clearColor));

    ImGui::End();

    EditorPanel::EndDockspace();
  }

  void Editor::NewScene()
  {
    m_ActiveScene = std::make_shared<Scene>();
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
  }

  void Editor::SaveAs()
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
        {
           NewScene();
        }

        if (ImGui::MenuItem("Open", "Ctrl+O"))
        {
          OpenScene();
        }

        if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
        {
          SaveAs();
        }

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
      ImGui::Begin("Render Status");
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

    if (ImGui::IsWindowHovered())
    {
      ImGuiIO& io = ImGui::GetIO();
      io.WantCaptureMouse = false;
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

    ImGui::End();

    ImGui::PopStyleVar();
      
  }

  void Editor::ViewportRefresh()
  {
    if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
      m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
      (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
    {
      m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

      m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    }
  }

  bool Editor::OnWindowResize(WindowResizeEvent& e)
  {
    return false;
  }

  bool Editor::OnMouseMovedEvent(MouseMovedEvent& e)
  {
    return false;
  }
}
