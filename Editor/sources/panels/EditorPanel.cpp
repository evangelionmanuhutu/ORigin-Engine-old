﻿#include "EditorPanel.h"
#include <imgui.h>

namespace Origin
{
  static bool guiDockingSpaceOpen = true;
  static bool guiMenuFullscreen = true;
  static bool guiMenuStyle = false;
  static bool guiRenderStatus = true;
  static bool guiDebugInfo = true;
  static bool guiImGuiDemoWindow = true;

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

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 2));

    ImGui::Begin("Origin", &guiDockingSpaceOpen, window_flags);

    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float minWindowSizeX = style.WindowMinSize.x;
    float minWindowSizeY = style.WindowMinSize.y;
    style.WindowMinSize.x = 320.0f;
    style.WindowMinSize.y = 130.0f;

    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    style.WindowMinSize.x = minWindowSizeX;
    style.WindowMinSize.y = minWindowSizeY;
  }

  void EditorPanel::EndDockspace()
  {
    ImGui::End();
    ImGui::PopStyleVar();
  }
}