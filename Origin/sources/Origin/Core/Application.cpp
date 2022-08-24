#include "pch.h"
#include "Application.h"


namespace Origin {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string title, bool maximize, bool fullscreen)
	{
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create(title, maximize, fullscreen));
		m_Window->SetEventCallback(OGN_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_GuiLayer = new GuiLayer();
		PushOverlay(m_GuiLayer);
	}
	

	void Application::Run() 
	{
		while (m_Window->loop()) {

			auto time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrame;
			m_LastFrame = time;

			if (!m_Minimized) {
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			m_GuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnGuiRender();
			m_GuiLayer->End();
			
			m_Window->OnUpdate();
			RenderCommand::Clear();
		}
	}

	void Application::PushOverlay(Layer* layer) 
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::PushLayer(Layer* layer) 
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(OGN_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(OGN_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		if(m_Window->loop())
			return false;

		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.getWidth() == 0 || e.getHeight() == 0) 
		{
			m_Minimized = true;
			return true;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.getWidth(), e.getHeight());

		return false;
	}
}