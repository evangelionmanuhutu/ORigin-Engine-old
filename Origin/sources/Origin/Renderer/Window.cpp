#include "pch.h"
#include "window.h"
#include "Icon.h"
#include "Origin\Utils\log.h"

#include "Origin\IO\Events\KeyEvent.h"
#include "Origin\IO\Events\MouseEvent.h"
#include "Origin\IO\Events\AppEvent.h"

#include "Origin\Core\Application.h"

#include <GLFW\glfw3.h>

namespace Origin {

	

	Window* Window::Create(const std::string& title, bool maximize, bool fullscreen) {
		return new Window(title, maximize, fullscreen);
	}

	Window::Window(const std::string& title, bool maximize, bool fullscreen)
		: Maximized(maximize), Fullscreen(fullscreen)
	{
		m_Data.Width = 1280;
		m_Data.Height = 600;
		m_Data.Title = title;
		init();
	}

	Window::~Window()
	{
		glfwTerminate();
		glfwDestroyWindow(m_Window);
	}

	bool Window::init()
	{
		// WINDOW CONFIGURATION
		if (!glfwInit()) {
			OGN_CORE_ERROR("ERROR :FAILED TO INITIALIZE GLFW");
			return false;
		}

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (Fullscreen) monitor = glfwGetPrimaryMonitor();
		else monitor = nullptr;

		bool WindowMaximized;
		if (Maximized) WindowMaximized = GLFW_TRUE;
		else WindowMaximized = GLFW_FALSE;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_MAXIMIZED, WindowMaximized);

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), monitor, nullptr);
		if (!m_Window) {
			OGN_CORE_ERROR("ERROR : FAILED TO CREATE WINDOW");
			glfwTerminate();
			return false;
		}

		//int max_width = GetSystemMetrics(SM_CXSCREEN);
		//int max_hieght = GetSystemMetrics(SM_CYSCREEN);
		//glfwSetWindowMonitor(m_Window, NULL, (max_width / 2) - (m_Data.Width / 2), (max_hieght / 2) - (m_Data.Height / 2), m_Data.Width, m_Data.Height, GLFW_DONT_CARE);

		OGN_CORE_INFO("Creating Window {0} ({1}, {2})", m_Data.Title, m_Data.Width, m_Data.Height);
		glfwMakeContextCurrent(m_Window);

		WindowCallbacks();
		return true;
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapInterval(setVSync);

		auto& app = Application::Get();
		if(!app.GetMinimized())
			glfwSwapBuffers(m_Window);
	}
	
	// Set Window Callbacks
	void Window::WindowCallbacks()
	{
		// Set init window user pointer first
		glfwSetWindowUserPointer(m_Window, &m_Data);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent e(width, height);
				data.EventCallback(e);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent e;
				data.EventCallback(e);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(key, 0);
					data.EventCallback(e);
					break;
				}

				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(key);
					data.EventCallback(e);
					break;
				}

				case GLFW_REPEAT:
				{
					KeyPressedEvent e(key, 1);
					data.EventCallback(e);
					break;
				}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent e(keycode);
				data.EventCallback(e);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e(button);
					data.EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e(button);
					data.EventCallback(e);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent e((float)xOffset, (float)yOffset);
				data.EventCallback(e);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent e((float)xPos, (float)yPos);
				data.EventCallback(e);
			});
	}

	void Window::SetIcon(const std::string& filepath)
	{
		Icon icon(m_Window, filepath);
	}
}
