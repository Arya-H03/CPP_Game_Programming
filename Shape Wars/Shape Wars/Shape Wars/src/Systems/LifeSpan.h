#pragma once

#include "EntityManager.h"

class LifeSpanSystem
{
private:

	EntityManager& entityManager;

	/*void UpdateSmallEnemyLifeSpan(const EntityVec& smallEnemies);
	void UpdateBulletsLifeSpan(const EntityVec& bullets);*/

public:

	LifeSpanSystem(EntityManager& entityManager):entityManager(entityManager) {}

	void HandleLifeSpanSystem();
};

