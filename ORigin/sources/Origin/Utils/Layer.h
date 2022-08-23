#pragma once
#include "pch.h"
#include "Origin\Core\OriginCore.h"
#include "Origin\IO\Events\Event.h"

#include "Time.h"

namespace Origin {

	class Layer
	{
	protected:
		std::string m_Name;

	public:
		Layer(const std::string& name = "layer");
		virtual ~Layer();
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_Name; }
	};
}
