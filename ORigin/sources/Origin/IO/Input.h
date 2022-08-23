#pragma once
#include "pch.h"
#include "Origin\Core\OriginCore.h"

namespace Origin
{
	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float>GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}