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
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void OnUpdate(Timestep time);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};

}