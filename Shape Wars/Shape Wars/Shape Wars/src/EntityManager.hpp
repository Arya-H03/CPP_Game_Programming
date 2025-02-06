#pragma once

#include <iostream>
#include "Entity.hpp"
#include <map>

using EntityVec = std::vector<std::shared_ptr<Entity>>;

class EntityManager
{
	EntityVec m_entities;
	EntityVec m_entitiesToAdd;
	std::map<std::string, EntityVec> m_entityMap;
	size_t m_totalEntities = 0;

	void RemoveDeadEntities(EntityVec& vec)
	{

	}

public:

	EntityManager() = default;
	
	void Update()
	{

	}

	std::shared_ptr<Entity> AddEntity(const std::string& tag)
	{
		auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

		m_entitiesToAdd.push_back(entity);

		if (m_entityMap.find(tag) == m_entityMap.end())
		{
			m_entityMap[tag] = EntityVec();
		}

		m_entityMap[tag].push_back(entity);

		return entity;
	}

	const EntityVec& GetEntities()
	{
		return m_entities;
	}

	const EntityVec& GetEntities(const std::string& tag)
	{
		if (m_entityMap.find(tag) == m_entityMap.end())
		{
			m_entityMap[tag] = EntityVec();
		}
		return m_entityMap[tag];
	}

	const std::map<std::string, EntityVec>& GetEntityMap()
	{
		return m_entityMap;
	}
};