#include "Editor.h"
#include "Origin/EntryPoint.h"
#include "panels/EditorTheme.h"

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

namespace Origin {

	void Editor::OnAttach()
  {
    FramebufferSpecification fbSpec;
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    m_Framebuffer = Framebuffer::Create(fbSpec);
    m_ActiveScene = std::make_shared<Scene>();

    m_SquareEntity = m_ActiveScene->CreateEntity("Entity 1");
    m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

    m_SquareEntity2 = m_ActiveScene->CreateEntity("Entity 2");
    m_SquareEntity2.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

    m_Camera = m_ActiveScene->CreateEntity("Camera A");
    m_Camera.AddComponent<CameraComponent>();

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

        auto& camera = GetComponent<CameraComponent>().Camera;
        float fov = glm::degrees(camera.GetPerspectiveFov());
        camera.SetPerspectiveFov(glm::radians(70.0F));
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

        auto& rotation = GetComponent<TransformComponent>().Rotation;
        //rotation.y += 0.8f * time;
      }
    };

    m_Camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
  }

  void Editor::OnUpdate(Timestep ts)
  {
    if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
      m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
      (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
    {
      m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

      m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    }

    // Render
    Renderer2D::ResetStats();
    m_Framebuffer->Bind();
    RenderCommand::Clear();
    RenderCommand::ClearColor(clearColor);

    // Update scene
    m_ActiveScene->OnUpdate(ts);

    m_Framebuffer->Unbind();
  }

  void Editor::OnGuiRender()
  {
    EditorPanel::BeginDockspace();
    EditorPanel::MenuBar();
    VpGui();
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

  bool Editor::OnWindowResize(WindowResizeEvent& e)
  {
    return false;
  }

  bool Editor::OnMouseMovedEvent(MouseMovedEvent& e)
  {
    return false;
  }
}
