#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "Entity.h"
#include "EntityMemoryPool.h"


class EntityManager
{
private:

    std::vector<Entity> m_entities;
    std::vector<Entity> m_entitiesToAdd;
    std::unordered_map<std::string, std::vector<Entity>> m_entityMap;

    void RemoveDeadEntities(std::vector<Entity>& vec);

public:

    EntityManager() = default;

    void Update();

    Entity AddEntity(const std::string& tag);

    const std::vector<Entity>& GetEntities() const;
    const std::vector<Entity>& GetEntities(const std::string& tag);

    const std::unordered_map<std::string, std::vector<Entity>>& GetEntityMap() const;
};
