#pragma once
#include "entt\entt.hpp"
#include "Origin\Utils\Time.h"

#include "EditorCamera.h"

namespace Origin
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateSpriteEntity(const std::string& name = std::string());
		Entity CreateCamera(const std::string& name = std::string());

		Entity GetPrimaryCameraEntity();

		void DestroyEntity(Entity entity);

		void OnUpdateRuntime(Timestep time);
		void OnUpdateEditor(Timestep time, EditorCamera& camera);

		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		template<typename T> void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}