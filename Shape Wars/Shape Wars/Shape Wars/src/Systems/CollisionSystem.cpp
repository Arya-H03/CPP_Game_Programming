#include "CollisionSystem.h"

//void Game::SCollision()
//{
//	for (auto& bullet : entityManager.GetEntities("Bullet"))
//	{
//		auto& bTransform = bullet->Get<CTransform>();
//		auto& bCollision = bullet->Get<CCollision>();
//		for (auto& enemy : entityManager.GetEntities("Enemy"))
//		{
//			auto& eTransform = enemy->Get<CTransform>();
//			auto& eCollision = enemy->Get<CCollision>();
//
//			if (bTransform.pos.Dist(eTransform.pos) < abs(bCollision.radius - eCollision.radius))
//			{
//				bullet->Destroy();
//				SpawnSmallEnemies(enemy);
//				enemy->Destroy();
//				audioData.PlayHitSFX(10);
//				score++;
//			}
//		}
//
//		for (auto& enemy : entityManager.GetEntities("SmallEnemy"))
//		{
//			auto& eTransform = enemy->Get<CTransform>();
//			auto& eCollision = enemy->Get<CCollision>();
//
//			if (bTransform.pos.Dist(eTransform.pos) < abs(bCollision.radius - eCollision.radius))
//			{
//				bullet->Destroy();
//				enemy->Destroy();
//				audioData.PlayHitSFX(10);
//				score++;
//
//			}
//		}
//	}
//
//	if (Player())
//	{
//		auto& pTransform = Player()->Get<CTransform>();
//		auto& pCollision = Player()->Get<CCollision>();
//
//		for (auto& enemy : entityManager.GetEntities("Enemy"))
//		{
//			auto& eTransform = enemy->Get<CTransform>();
//			auto& eCollision = enemy->Get<CCollision>();
//
//			if (pTransform.pos.Dist(eTransform.pos) < abs(pCollision.radius - eCollision.radius))
//			{
//
//				enemy->Destroy();
//				Player()->Destroy();
//				audioData.PlayDeathSFX(15);
//
//			}
//		}
//
//		for (auto& smallEnemy : entityManager.GetEntities("SmallEnemy"))
//		{
//			auto& eTransform = smallEnemy->Get<CTransform>();
//			auto& eCollision = smallEnemy->Get<CCollision>();
//
//			if (pTransform.pos.Dist(eTransform.pos) < abs(pCollision.radius - eCollision.radius))
//			{
//
//				smallEnemy->Destroy();
//				Player()->Destroy();
//				audioData.PlayDeathSFX(15);
//
//			}
//		}
//	}
//
//}