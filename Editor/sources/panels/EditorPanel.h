#pragma once
#include <Origin.h>

namespace Origin {

	class EditorPanel
	{
	public:
		static void Viewport(const std::shared_ptr<Framebuffer>& framebuffer, OrthoCameraController& cameraController);
		static void BeginViewport(const std::shared_ptr<Framebuffer>& framebuffer, OrthoCameraController& cameraController);
		static void EndViewport(const std::shared_ptr<Framebuffer>& framebuffer);

		static void MenuBar();
		static void BeginDockspace();
		static void EndDockspace();
	};

}