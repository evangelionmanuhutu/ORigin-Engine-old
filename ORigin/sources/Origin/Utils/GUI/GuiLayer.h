#pragma once
#include "Origin\Core\OriginCore.h"
#include "Origin\Utils\Layer.h"

#include "Origin/IO/Events/AppEvent.h"
#include "Origin/IO/Events/KeyEvent.h"
#include "Origin/IO/Events/MouseEvent.h"

#include <imgui.h>
#include <backends\imgui_impl_glfw.h>
#include <backends\imgui_impl_opengl3.h>

namespace Origin {
	
	class GuiLayer : public Layer
	{
	public:
		GuiLayer() : Layer("Gui Layer") {}
		~GuiLayer();

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};

}