#pragma once
#include <Origin.h>

namespace Origin {

	class EditorViewport
	{
	public:
		EditorViewport() {}
		~EditorViewport();

		void Refresh(const std::shared_ptr<Framebuffer>& framebuffer, OrthoCameraController& cameraController);
		void GuiViewport(const std::shared_ptr<Framebuffer>& framebuffer);

		int GetMouseX() const { return mouseX; }
		int GetMouseY() const { return mouseY; }

		glm::vec2 GetWindowSize() { return m_ViewportSize; }
		float GetWidth() { return m_ViewportSize.x; }
		float GetHeight() { return m_ViewportSize.y; }

	private:
		glm::vec2 m_ViewportSize;
		int mouseX, mouseY;
	};

	class EditorPanel
	{
	public:
		static void MenuBar();
		static void BeginDockspace();
		static void EndDockspace();
	};
}