#include "LifeSpan.h"

void LifeSpanSystem::UpdateSmallEnemyLifeSpan(const EntityVec& smallEnemies)
{
	for (auto& smallEnemy : smallEnemies)
	{
		smallEnemy->Get<CLifeSpan>().remaining--;
		if (smallEnemy->Get<CLifeSpan>().remaining <= 0) smallEnemy->Destroy();
	}
}

void LifeSpanSystem::UpdateBulletsLifeSpan(const EntityVec& bullets)
{
	for (auto& bullet : bullets)
	{
		bullet->Get<CLifeSpan>().remaining--;
		if (bullet->Get<CLifeSpan>().remaining <= 0) bullet->Destroy();
	}
}

void LifeSpanSystem::HandleLifeSpanSystem()
{
	UpdateSmallEnemyLifeSpan(entityManager.GetEntities("SmallEnemy"));
	UpdateBulletsLifeSpan(entityManager.GetEntities("SmallEnemy"));
}



