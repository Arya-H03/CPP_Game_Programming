#pragma once
#include "ConfigData.hpp"
#include "EntityManager.h"
#include "Vec2.hpp"


class MovementSystem
{
private:
	const ConfigData& configData;
	EntityManager& entityManager;

	void UpdatePlayerMovement(Entity& player);
	void UpdateBulletMovement(Entity& bullet);
	void UpdateEnemyMovement(Entity& enemy);
	void UpdateSmallEnemyMovement(Entity& smallEnemy);

public:
	MovementSystem(const ConfigData& configData,EntityManager& entityManager): configData(configData), entityManager(entityManager)
	{}

	void HandleMovementSystem();
};

