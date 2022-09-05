#include "pch.h"
#include "SceneSerializer.h"
#include "Component\Component.h"

#include <yaml-cpp\yaml.h>


namespace Origin
{
	SceneSerializer::SceneSerializer(const std::shared_ptr<Scene>& scene)
		:m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity& entity)
	{
		out << YAML::BeginMap; // Entity Map
		out << YAML::Key << "Entity" << YAML::Value << "123456784212";
		out << YAML::EndMap;	 // !Entity Map

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "Tag Component";
			out << YAML::BeginMap; // Tag Component Map

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // !Tag Component Map
		}

	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entites" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID)
		{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;

				SerializeEntity(out, entity);
		});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		return false;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		return false;
	}
}

