#pragma once
#include "Origin\Core\OriginCore.h"
#include "Origin\IO\Events\Event.h"

#include <string>
#include <GLFW\glfw3.h>

namespace Origin {

	struct WindowData
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		std::function<void(Event&)> EventCallback;
	};

	class Window
	{
	private:
		GLFWwindow* m_Window;
		WindowData m_Data;
		bool Fullscreen;
		bool Maximized;

	public:
		bool setVSync;
		int m_Close = 0;

		Window(const std::string& title, bool maximize, bool fullscreen);
		~Window();

		bool init();
		void SetEventCallback(const std::function<void(Event&)>& callback)
		{
			m_Data.EventCallback = callback;
		};

		void OnUpdate();
		bool loop() { return glfwWindowShouldClose(m_Window) == m_Close; }
		void Close() { m_Close = 1; }
		void WindowCallbacks();

		void SetIcon(const std::string& filepath);

		inline uint32_t GetWidth() { return m_Data.Width; }
		inline uint32_t GetHeight() { return m_Data.Height; }

		static Window* Create(const std::string& title, bool maximize, bool fullscreen);
		GLFWwindow* getWindow() { return m_Window; }
	};
}

