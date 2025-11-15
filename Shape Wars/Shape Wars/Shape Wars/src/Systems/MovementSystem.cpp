#include "MovementSystem.h"
#include "Vec2.hpp"

MovementSystem::MovementSystem(const ConfigData& configData) :configData(configData)
{
}

void MovementSystem::UpdatePlayerMovement(Entity& player)
{
	CTransform& cTransform = player.Get<CTransform>();
	CInput& cInput = player.Get<CInput>();

	cTransform.velocity.y = cInput.down - cInput.up;
	cTransform.velocity.x = cInput.right - cInput.left;

	if (cTransform.velocity != Vec2<float>::Zero) cTransform.velocity = cTransform.velocity.Normalize();

	//Bound Check (Change Later)
	if ((cTransform.pos.y - configData.playerShapeRadius <= 0 && cTransform.velocity.y < 0) || cTransform.pos.y + configData.playerShapeRadius >= configData.windowH && cTransform.velocity.y > 0)
	{
		cTransform.velocity.y = 0;
	}

	if ((cTransform.pos.x - configData.playerShapeRadius <= 0 && cTransform.velocity.x < 0) || (cTransform.pos.x + configData.playerShapeRadius >= configData.windowW && cTransform.velocity.x > 0))
	{
		cTransform.velocity.x = 0;
	}

	cTransform.pos += cTransform.velocity * cTransform.speed;
}

void MovementSystem::UpdateBulletMovement(Entity& bullet)
{
	CTransform& cTransform = bullet.Get<CTransform>();
	cTransform.pos += cTransform.velocity.Normalize() * cTransform.speed;
}

void MovementSystem::UpdateEnemyMovement(Entity& enemy)
{
	CTransform& cTransform = enemy.Get<CTransform>();
	cTransform.pos += cTransform.velocity.Normalize() * cTransform.speed;

	//Bound Check (Change Later)
	if (cTransform.pos.y - configData.enemyShapeRadius <= 0 || cTransform.pos.y + configData.enemyShapeRadius >= configData.windowH)
	{
		cTransform.velocity.y *= -1;
	}

	if (cTransform.pos.x - configData.enemyShapeRadius <= 0 || cTransform.pos.x + configData.enemyShapeRadius >= configData.windowW)
	{
		cTransform.velocity.x *= -1;
	}
}

void MovementSystem::UpdateSmallEnemyMovement(Entity& smallEnemy)
{
	CTransform& cTransform = smallEnemy.Get<CTransform>();
	cTransform.pos += cTransform.velocity.Normalize() * cTransform.speed;

	//Bound Check (Change Later)
	if (cTransform.pos.y - configData.enemyShapeRadius <= 0 || cTransform.pos.y + configData.enemyShapeRadius >= configData.windowH)
	{
		cTransform.velocity.y *= -1;
	}

	if (cTransform.pos.x - configData.enemyShapeRadius <= 0 || cTransform.pos.x + configData.enemyShapeRadius >= configData.windowW)
	{
		cTransform.velocity.x *= -1;
	}
}
