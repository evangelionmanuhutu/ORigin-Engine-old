#include "EditorPanel.h"
#include <imgui.h>

namespace Origin
{
  static bool guiDockingSpaceOpen = true;
  static bool guiMenuFullscreen = true;
  static bool guiMenuStyle = true;
  static bool guiViewportOpen = true;
  static bool guiRenderStatus = true;
  static bool guiDebugInfo = true;
  static bool guiImGuiDemoWindow = true;
  glm::vec2 viewportSize = { 0.0f, 0.0f };
  glm::vec2 m_ViewportBounds[2];


  EditorViewport::~EditorViewport()
  {
  }

  void EditorViewport::Refresh(const std::shared_ptr<Framebuffer>& framebuffer, OrthoCameraController& cameraController, const glm::vec4& clearColor)
  {
    if (FramebufferSpecification spec = framebuffer->GetSpecification();
      viewportSize.x > 0.0f && viewportSize.y > 0.0f &&
      (spec.Width != viewportSize.x || spec.Height != viewportSize.y))
    {
      framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
      cameraController.OnResize(viewportSize.x, viewportSize.y);
    }
    framebuffer->Bind();

    RenderCommand::ClearColor(clearColor);
    RenderCommand::Clear();
  }


  void EditorViewport::GuiViewport(const std::shared_ptr<Framebuffer>& framebuffer)
  {
    if (guiImGuiDemoWindow)
      ImGui::ShowDemoWindow(&guiImGuiDemoWindow);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar
      | ImGuiWindowFlags_NoScrollWithMouse
      | ImGuiWindowFlags_NoCollapse;

    if (guiViewportOpen)
    {
      ImGui::Begin("Viewport", &guiViewportOpen, window_flags);

      auto viewportOffset = ImGui::GetCursorPos();

      if (ImGui::IsWindowHovered())
      {
        ImGuiIO& io = ImGui::GetIO();
        io.WantCaptureMouse = false;
      }

      ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
      viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

      uint32_t viewportID = framebuffer->GetColorAttachmentRendererID();
      ImGui::Image((void*)viewportID, ImVec2(viewportSize.x, viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

      auto windowSize = ImGui::GetWindowSize();
      m_ViewportSize = viewportSize;

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
      mouseY = (int)my;

      if (mouseX < 0) mouseX = 0;
      else if (mouseX > (int)viewportSize.x) mouseX = (int)viewportSize.x;
      if (mouseY < 0) mouseY = 0;
      else if (mouseY > (int)viewportSize.y) mouseY = (int)viewportSize.y;

      ImGui::End();
    }
  }


  // Editor Panel

  void EditorPanel::MenuBar()
  {
    ImGuiIO& io = ImGui::GetIO();

    if (ImGui::BeginMenuBar())
    {

      static bool ExitWindow = false;
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("Exit")) {
          ExitWindow = true;
        }
        ImGui::EndMenu();
      }

    	ImGuiStyle& style = ImGui::GetStyle();
      GLFWwindow* window = Application::Get().GetWindow().getWindow();
      
      if (ExitWindow)
      {
        style.WindowPadding.x = 20;
        style.FramePadding.x = 6;

        int width, height;
        glfwGetWindowSize(window, &width, &height);
        ImGui::SetNextWindowPos(ImVec2((float)width / 2.0f, (float)height / 2.0f));
        ImGui::SetNextWindowSize(ImVec2(140.0f, 64.0f));

        ImGui::Begin("Quit", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        ImGui::Text("Are you sure ?");

        if (ImGui::Button(" Yes "))
          Application::Get().Close();

        ImGui::SameLine();
        if(ImGui::Button(" No ")) ExitWindow = false;

        ImGui::End();
      }

      else
      {
        style.WindowPadding.x = 2;
        style.FramePadding.x = 1;
      }

      if (ImGui::BeginMenu("Window"))
      {
        ImGui::MenuItem("Viewport", NULL, &guiViewportOpen);
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

    if (guiDebugInfo)
    {
      ImGui::Begin("Debug Info");
      ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::Text("OpenGL Version : (%s)", glGetString(GL_VERSION));
      ImGui::Text("ImGui version : (%s)", IMGUI_VERSION);
      ImGui::Checkbox("vSync", &Application::Get().SetVSync);
      ImGui::Separator();
      ImGui::End();
    }

    if (guiMenuStyle)
    {
      ImGui::Begin("Style Editor");
      ImGui::ShowStyleEditor();
      ImGui::End();
    }
  }

  void EditorPanel::BeginDockspace()
  {
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGui::PopStyleVar(2);

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
      window_flags |= ImGuiWindowFlags_NoBackground;

    if (guiDockingSpaceOpen == false)
      Application::Get().Close();

    ImGui::Begin("Origin", &guiDockingSpaceOpen, window_flags);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
  }

  void EditorPanel::EndDockspace()
  {
    ImGui::End();
  }
}