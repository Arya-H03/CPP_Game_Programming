#pragma once

#include "EntityManager.hpp"

class LifeSpan
{
private:

	void UpdateSmallEnemyLifeSpan(const EntityVec& smallEnemies);
	void UpdateBulletsLifeSpan(const EntityVec& bullets);

public:

	void HandleLifeSpanSystem(EntityManager& entityManager);
};

