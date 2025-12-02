#pragma once
#include "EntityManager.hpp"
#include "MyEvent.hpp"

class CollisionSystem
{
private:

	EntityManager& entityManager;
	size_t playerID;

	void HandleBulletCollision();
	void HandlePlayerCollision();

public:
	CollisionSystem(EntityManager& entityManager, size_t playerID) :entityManager(entityManager), playerID(playerID) {}

	MyEvent<void(Entity& bullet, Entity& enemy)> onBulletCollisionWithEnemies;
	MyEvent<void(Entity& bullet, Entity& enemy)> onBulletCollisionWithSmallEnemy;
	MyEvent<void(Entity& enemy)> onPlayerCollisionWithEnemies;

	void HandleCollisionSystem();
	void ResetPlayer(Entity* newPlayer);

}; 

