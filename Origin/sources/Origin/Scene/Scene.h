#pragma once
#include "entt\entt.hpp"
#include "Origin\Utils\Time.h"

namespace Origin
{
	class Entity;
	
	class Scene
	{
	public:
		Scene();

		Entity CreateEntity(const std::string& name);
		entt::registry& Reg() { return m_Registry; }
		void OnUpdate(Timestep time);

	private:
		entt::registry m_Registry;

		friend class Entity;
	};

}