#include "EntityMemoryPool.h"
#include "Entity.h"
#include <stdexcept>

EntityMemoryPool::EntityMemoryPool(size_t maxSize) : m_numberOfEntities(maxSize), m_tags(maxSize), m_active(maxSize, false)
{
	std::apply(

		[&](auto&... componentVector)
		{
			(..., componentVector.resize(maxSize));
		},
		m_componentPool
	);
}

size_t EntityMemoryPool::GetNextFreeEntityIndex()
{
	for (size_t index = 0; index < m_active.size(); ++index)
	{
		if (m_active[index] == false) return index;
	}

	throw std::runtime_error("Entity pool exhausted");
}

EntityMemoryPool& EntityMemoryPool::Instance()
{
	static EntityMemoryPool instance(2000);
	return instance;
}

const std::string& EntityMemoryPool::GetTag(size_t entityID) const
{
	return m_tags[entityID];
}
Entity EntityMemoryPool::AddEntity(const std::string& tag)
{
	size_t index = GetNextFreeEntityIndex();
	m_active[index] = true;
	m_tags[index] = tag;

	std::apply([&](auto&... componentVectors)
		{
			(..., (componentVectors[index] = {}));
		}, m_componentPool);


	return Entity(index);
}

void EntityMemoryPool::DestroyEntity(size_t entityID)
{
	m_active[entityID] = false;

	std::apply([&](auto&... componentVectors)
		{
			(..., (componentVectors[entityID] = {}));
		}, m_componentPool);
}

bool EntityMemoryPool::IsEntityActive(size_t entityID)
{
	return m_active[entityID];
}

const EntityComponentVectorTuple& EntityMemoryPool::GetComponentPool() const
{
	return m_componentPool;
}
