#include "pch.h"
#include "Input.h"
#include "Origin\Core\Application.h"

namespace Origin
{
	bool Input::IsKeyPressed(int keycode)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().getWindow());
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().getWindow());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	std::pair<float, float>Input::GetMousePosition()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().getWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return { (float)xPos, float(yPos) };
	}

	float Input::GetMouseX()
	{
		auto v = GetMousePosition();
		return std::get<0>(v);
	}

	float Input::GetMouseY()
	{
		auto v = GetMousePosition();
		return std::get<0>(v);
	}
}