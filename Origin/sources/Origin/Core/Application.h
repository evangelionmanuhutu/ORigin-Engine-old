#pragma once
#include "OriginCore.h"
#include "Origin\IO\Input.h"

#include "Origin\Renderer\window.h"
#include "Origin\IO\Events\AppEvent.h"

#include "Origin\Utils\Time.h"
#include "Origin\Utils\Layer.h"
#include "Origin\Utils\LayerStack.h"
#include "Origin\Utils\GUI\GuiLayer.h"

#include "Origin\Renderer\Renderer.h"

namespace Origin {

	class Application
	{
	public:
		Application(const std::string title, bool maximize, bool fullscreen);
		virtual ~Application()
		{
			Renderer::Shutdown();
		}

		void OnEvent(Event& e);
		void Run();
		void Close() { m_Window->Close(); }

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		bool SetVSync;

		inline static Application& Get() { return *s_Instance; }
		inline bool GetMinimized() { return m_Minimized; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		static Application* s_Instance;
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		bool m_Minimized = false;
		
		LayerStack m_LayerStack;
		std::unique_ptr<Window> m_Window;
		GuiLayer* m_GuiLayer;
		float m_LastFrame = 0.0f;
	};

	Application* CreateApplication();
}