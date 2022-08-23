#include "pch.h"

#include "GuiLayer.h"
#include "Origin\Core\OriginCore.h"
#include "Origin\Core\Application.h"

namespace Origin {

	void GuiLayer::OnAttach()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigViewportsNoDecoration = true;
		Application& app = Application::Get();

		ImGui_ImplGlfw_InitForOpenGL(app.GetWindow().getWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	GuiLayer::~GuiLayer()
	{
		OnDetach();
	}

	void GuiLayer::OnUpdate(Timestep ts)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = m_Time > 0.0f ? (ts - m_Time) : (1.0f / 60.0f);
		m_Time = m_Time - ts;
	}

	void GuiLayer::Begin()
	{
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
	}

	void GuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();

		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_context_current = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_context_current);
		}
	}

	void GuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GuiLayer::OnEvent(Event& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		e.Handled |= e.isInCategory(EventCategoryMouse) & io.WantCaptureMouse;
		e.Handled |= e.isInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
	}
}