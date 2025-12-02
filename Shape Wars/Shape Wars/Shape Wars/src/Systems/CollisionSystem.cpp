#include "CollisionSystem.h"


void CollisionSystem::HandleBulletCollision()
{
	for (auto& bullet : entityManager.GetEntities("Bullet"))
	{
		auto& bTransform = bullet->Get<CTransform>();
		auto& bCollision = bullet->Get<CCollision>();
		for (auto& enemy : entityManager.GetEntities("Enemy"))
		{
			auto& eTransform = enemy->Get<CTransform>();
			auto& eCollision = enemy->Get<CCollision>();

			if (bTransform.pos.Dist(eTransform.pos) < abs(bCollision.radius - eCollision.radius))
			{
				onBulletCollisionWithEnemies.Invoke(*bullet.get(), *enemy.get());
			}
		}

		for (auto& enemy : entityManager.GetEntities("SmallEnemy"))
		{
			auto& eTransform = enemy->Get<CTransform>();
			auto& eCollision = enemy->Get<CCollision>();

			if (bTransform.pos.Dist(eTransform.pos) < abs(bCollision.radius - eCollision.radius))
			{
				onBulletCollisionWithSmallEnemy.Invoke(*bullet.get(), *enemy.get());
			}
		}
	}
}

void CollisionSystem::HandlePlayerCollision()
{
	Entity* player = entityManager.FindEntityByID(playerID);
	if (!player) return;

	auto& pTransform = player->Get<CTransform>();
	auto& pCollision = player->Get<CCollision>();

	for (auto& enemy : entityManager.GetEntities("Enemy"))
	{
		
		auto& eTransform = enemy->Get<CTransform>();
		auto& eCollision = enemy->Get<CCollision>();
		
		if (pTransform.pos.Dist(eTransform.pos) < abs(pCollision.radius - eCollision.radius))
		{
			
			onPlayerCollisionWithEnemies.Invoke(*enemy.get());
		}
	}

	for (auto& smallEnemy : entityManager.GetEntities("SmallEnemy"))
	{
		auto& eTransform = smallEnemy->Get<CTransform>();
		auto& eCollision = smallEnemy->Get<CCollision>();

		if (pTransform.pos.Dist(eTransform.pos) < abs(pCollision.radius - eCollision.radius))
		{
			onPlayerCollisionWithEnemies.Invoke(*smallEnemy.get());
		}
	}
}

void CollisionSystem::HandleCollisionSystem()
{
	HandleBulletCollision();
	HandlePlayerCollision();
}

void CollisionSystem::ResetPlayer(Entity* newPlayer)
{
	playerID = newPlayer->Id();
}

