#pragma once
#include <entt.hpp>

namespace Origin
{

	class Scene
	{
	public:
		Scene();
		~Scene();

	private:
		entt::registry m_Registry;
	};

}