#include "Entity.h"
#include "EntityMemoryPool.h"

Entity::Entity(size_t id)
    : m_id(id)
{
}

bool Entity::IsActive() const
{
    return EntityMemoryPool::Instance().IsEntityActive(m_id);
}

void Entity::Destroy() const
{
    EntityMemoryPool::Instance().DestroyEntity(m_id);
}

size_t Entity::Id() const
{
    return m_id;
}

const std::string& Entity::Tag() const
{
    return EntityMemoryPool::Instance().GetTag(m_id);
}
