#pragma once
#include "pch.h"
#include "KeyCodes.h"
#include "MouseCodes.h"

#include <glm\glm.hpp>

namespace Origin
{
	class Input
	{
	public:
		static bool IsKeyPressed(const KeyCode keycode);
		static bool IsMouseButtonPressed(const MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}