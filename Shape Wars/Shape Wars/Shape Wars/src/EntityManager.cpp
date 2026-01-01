#include "EntityManager.h"

void EntityManager::RemoveDeadEntities(std::vector<Entity>& vec)
{
    vec.erase(
        std::remove_if(vec.begin(), vec.end(),
            [](const Entity& e)
            {
                return !e.IsActive();
            }),
        vec.end()
    );
}

void EntityManager::Update()
{
    for (auto& e : m_entitiesToAdd)
    {
        m_entities.push_back(e);
    }
    m_entitiesToAdd.clear();

    RemoveDeadEntities(m_entities);

    for (auto& [tag, entityVec] : m_entityMap)
    {
        RemoveDeadEntities(entityVec);
    }
}

Entity EntityManager::AddEntity(const std::string& tag)
{
    Entity entity = EntityMemoryPool::Instance().AddEntity(tag);

    m_entitiesToAdd.push_back(entity);
    m_entityMap[tag].push_back(entity);

    return entity;
}

const std::vector<Entity>& EntityManager::GetEntities() const
{
    return m_entities;
}

const std::vector<Entity>& EntityManager::GetEntities(const std::string& tag)
{
    return m_entityMap[tag]; 
}

const std::unordered_map<std::string, std::vector<Entity>>&
EntityManager::GetEntityMap() const
{
    return m_entityMap;
}
