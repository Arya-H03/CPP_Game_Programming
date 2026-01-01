#pragma once
#include <tuple>
#include <vector>
#include "Components.hpp"

typedef std::tuple<

	std::vector<CTransform>,
	std::vector<CShape>,
	std::vector<CCollision>,
	std::vector<CScore>,
	std::vector<CLifeSpan>,
	std::vector<CInput>,
	std::vector<CCoord>,
	std::vector<CText>

> EntityComponentVectorTuple;

class Entity;

class EntityMemoryPool
{
private:

	size_t m_numberOfEntities;
	EntityComponentVectorTuple m_componentPool;
	std::vector<std::string> m_tags;
	std::vector<bool> m_active;

	EntityMemoryPool(size_t maxSize);

	size_t GetNextFreeEntityIndex();

public:

	static EntityMemoryPool& Instance();

	const std::string& GetTag(size_t entityID) const;

	template<typename T>
	T& GetComponent(size_t entityID)
	{
		return std::get<std::vector<T>>(m_componentPool)[entityID];
	}

	Entity AddEntity(const std::string& tag);

	void DestroyEntity(size_t entityID);

	bool IsEntityActive(size_t entityID);

};

