#include "LifeSpan.h"

void LifeSpan::UpdateSmallEnemyLifeSpan(const EntityVec& smallEnemies)
{
	for (auto& smallEnemy : smallEnemies)
	{
		smallEnemy->Get<CLifeSpan>().remaining--;
		if (smallEnemy->Get<CLifeSpan>().remaining <= 0) smallEnemy->Destroy();
	}
}

void LifeSpan::UpdateBulletsLifeSpan(const EntityVec& bullets)
{
	for (auto& bullet : bullets)
	{
		bullet->Get<CLifeSpan>().remaining--;
		if (bullet->Get<CLifeSpan>().remaining <= 0) bullet->Destroy();
	}
}

void LifeSpan::HandleLifeSpanSystem(EntityManager& entityManager)
{
	UpdateSmallEnemyLifeSpan(entityManager.GetEntities("SmallEnemy"));
	UpdateBulletsLifeSpan(entityManager.GetEntities("SmallEnemy"));
}



