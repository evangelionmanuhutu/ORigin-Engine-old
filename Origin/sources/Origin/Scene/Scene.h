#pragma once
#include "Origin\Utils\Time.h"
#include "entt.hpp"

namespace Origin
{
	class Entity;
	
	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		entt::registry& Reg() { return m_Registry; }
		void OnUpdate(Timestep time);

	private:
		entt::registry m_Registry;

		friend class Entity;
	};

}