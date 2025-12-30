#pragma once
#include "ConfigData.hpp"
#include "EntityManager.hpp"
#include "Vec2.hpp"


class MovementSystem
{
private:
	const ConfigData& configData;
	size_t playerID;
	EntityManager& entityManager;

	void UpdatePlayerMovement(Entity& player);
	void UpdateBulletMovement(Entity& bullet);
	void UpdateEnemyMovement(Entity& enemy);
	void UpdateSmallEnemyMovement(Entity& smallEnemy);

public:
	MovementSystem(const ConfigData& configData,EntityManager& entityManager, size_t playerID): configData(configData), entityManager(entityManager), playerID(playerID)
	{}

	void HandleMovementSystem();
	void ResetPlayer(Entity* newPlayer);
};

