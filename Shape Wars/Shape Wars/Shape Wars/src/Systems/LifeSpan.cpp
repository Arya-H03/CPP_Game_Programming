//#include "LifeSpan.h"
//
//void LifeSpanSystem::UpdateSmallEnemyLifeSpan(const EntityVec& smallEnemies)
//{
//	for (auto& smallEnemy : smallEnemies)
//	{
//		smallEnemy->GetComponent<CLifeSpan>().remaining--;
//		if (smallEnemy->GetComponent<CLifeSpan>().remaining <= 0) smallEnemy->Destroy();
//	}
//}
//
//void LifeSpanSystem::UpdateBulletsLifeSpan(const EntityVec& bullets)
//{
//	for (auto& bullet : bullets)
//	{
//		bullet->GetComponent<CLifeSpan>().remaining--;
//		if (bullet->GetComponent<CLifeSpan>().remaining <= 0) bullet->Destroy();
//	}
//}
//
//void LifeSpanSystem::HandleLifeSpanSystem()
//{
//	UpdateSmallEnemyLifeSpan(entityManager.GetEntities("SmallEnemy"));
//	UpdateBulletsLifeSpan(entityManager.GetEntities("SmallEnemy"));
//}
//
//
//
