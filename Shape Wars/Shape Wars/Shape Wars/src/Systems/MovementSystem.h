#pragma once
#include "ConfigData.hpp"
#include "EntityManager.hpp"


class MovementSystem
{
private:
	const ConfigData& configData;

	void UpdatePlayerMovement(Entity& player);
	void UpdateBulletMovement(Entity& bullet);
	void UpdateEnemyMovement(Entity& enemy);
	void UpdateSmallEnemyMovement(Entity& smallEnemy);

public:
	MovementSystem(const ConfigData& configData);

	void HandleMovementSystem(EntityManager& entityManager);

};

