#include "Editor.h"
#include "Origin/EntryPoint.h"
#include "panels/EditorTheme.h"

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace Origin {

	void Editor::OnAttach()
  {
    EditorTheme::Apply();
    FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = Framebuffer::Create(fbSpec);
    m_ActiveScene = std::make_shared<Scene>();

    m_SquareEntity = m_ActiveScene->CreateEntity("Square");
    m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.5f, 0.0f, 1.0f });

    m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
    m_CameraEntity.AddComponent<CameraComponent>();
    
    
    m_SecondCameraEntity = m_ActiveScene->CreateEntity("Clip-Camera Entity");
    auto& cc = m_SecondCameraEntity.AddComponent<CameraComponent>();
    cc.Primary = false;
  }

  void Editor::OnUpdate(Timestep ts)
  {
    VpRefresh();
    Renderer2D::ResetStats();

    m_ActiveScene->OnUpdate(ts);
  }

  void Editor::OnGuiRender()
  {
    EditorPanel::BeginDockspace();
    EditorPanel::MenuBar();
    VpGui();

    if (m_SquareEntity) {
      ImGui::Begin("Quad");
      auto& Tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
      ImGui::Text("%s", Tag.c_str());

      auto& squarePosition = m_SquareEntity.GetComponent<TransformComponent>().Transform[3];
      ImGui::DragFloat2("Postion", glm::value_ptr(squarePosition), 0.01f);

      auto& squareColor = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
      ImGui::ColorPicker4("Color", glm::value_ptr(squareColor));

      ImGui::End();
    }

    if (m_CameraEntity)
    {
      ImGui::Begin("Camera");

      auto& Tag = m_CameraEntity.GetComponent<TagComponent>().Tag;
      ImGui::Text("%s", Tag.c_str());

      auto& Transform = m_CameraEntity.GetComponent<TransformComponent>().Transform[3];
      ImGui::DragFloat3("Transform", glm::value_ptr(Transform), 0.01f);
      if (ImGui::Checkbox("Primary", &CamPrimary))
      {
        m_CameraEntity.GetComponent<CameraComponent>().Primary = CamPrimary;
        m_SecondCameraEntity.GetComponent<CameraComponent>().Primary = !CamPrimary;
      }

      ImGui::Text("Camera Settings");
      auto& camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
      float OrthoSize = camera.GetOrthographicSize();

      if (ImGui::DragFloat("Projection Size", &OrthoSize, 0.01f, 1.0f, 100.0f))
        camera.SetOrthographicSize(OrthoSize);

      ImGui::End();
    }

    EditorPanel::EndDockspace();
  }

  void Editor::VpRefresh()
  {
    if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
      m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
      (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
    {
      m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

      m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    }
    m_Framebuffer->Bind();

    RenderCommand::ClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    RenderCommand::Clear();
  }

  void Editor::VpGui()
  {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar
      | ImGuiWindowFlags_NoScrollWithMouse
      | ImGuiWindowFlags_NoCollapse;
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

    int mouseX = (int)mx;
    int mouseY = (int)my;

    if (mouseX < 0) mouseX = 0;
    else if (mouseX > (int)viewportSize.x) mouseX = (int)viewportSize.x;
    if (mouseY < 0) mouseY = 0;
    else if (mouseY > (int)viewportSize.y) mouseY = (int)viewportSize.y;

    ImGui::End();
      
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
