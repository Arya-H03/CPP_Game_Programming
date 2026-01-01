#pragma once
#include "EntityManager.h"
#include "MyEvent.hpp"

class CollisionSystem
{
private:

	EntityManager& entityManager;

	void HandleBulletCollision();
	void HandlePlayerCollision();

public:
	CollisionSystem(EntityManager& entityManager) :entityManager(entityManager) {}

	MyEvent<void(Entity& bullet, Entity& enemy)> onBulletCollisionWithEnemies;
	MyEvent<void(Entity& bullet, Entity& enemy)> onBulletCollisionWithSmallEnemy;
	MyEvent<void(Entity& enemy)> onPlayerCollisionWithEnemies;

	void HandleCollisionSystem();
	void ResetPlayer(Entity* newPlayer);

}; 

