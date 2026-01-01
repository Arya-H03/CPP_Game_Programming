#include "MovementSystem.h"

void MovementSystem::HandleMovementSystem()
{
	for (Entity entity : entityManager.GetEntities())
	{
		if (!entity.HasComponent<CTransform>()) continue;

		CTransform& transform = entity.GetComponent<CTransform>();

		if (entity.HasComponent<CInput>())
		{
			CInput& input = entity.GetComponent<CInput>();

			transform.velocity.y = input.down - input.up;
			transform.velocity.x = input.right - input.left;
		}

		if (transform.velocity != Vec2<float>::Zero) transform.velocity = transform.velocity.Normalize();

		if ((transform.pos.y - configData.playerShapeRadius <= 0 && transform.velocity.y < 0) || transform.pos.y + configData.playerShapeRadius >= configData.windowH && transform.velocity.y > 0)
		{
			transform.velocity.y = 0;
		}

		if ((transform.pos.x - configData.playerShapeRadius <= 0 && transform.velocity.x < 0) || (transform.pos.x + configData.playerShapeRadius >= configData.windowW && transform.velocity.x > 0))
		{
			transform.velocity.x = 0;
		}

		transform.pos += transform.velocity * transform.speed;
		transform.angle += 1.0f;

		if (entity.HasComponent<CShape>())
		{
			CShape& shape = entity.GetComponent<CShape>();

			shape.circle.setPosition(transform.pos);
			shape.circle.setRotation(sf::degrees(transform.angle));
		}

	}


	/*for (auto& bullet : entityManager.GetEntities("Bullet"))
	{
		UpdateBulletMovement(bullet);
	}*/


	/*for (auto& enemy : entityManager.GetEntities("Enemy"))
	{
		UpdateEnemyMovement(*enemy);
	}


	for (auto& smallEnemy : entityManager.GetEntities("SmallEnemy"))
	{
		UpdateSmallEnemyMovement(*smallEnemy);
	}*/
}

void MovementSystem::UpdatePlayerMovement(Entity& player)
{
	//CTransform& cTransform = player.GetComponent<CTransform>();
	//CInput& cInput = player.GetComponent<CInput>();

	//cTransform.velocity.y = cInput.down - cInput.up;
	//cTransform.velocity.x = cInput.right - cInput.left;

	//if (cTransform.velocity != Vec2<float>::Zero) cTransform.velocity = cTransform.velocity.Normalize();

	////Bound Check (Change Later)
	//if ((cTransform.pos.y - configData.playerShapeRadius <= 0 && cTransform.velocity.y < 0) || cTransform.pos.y + configData.playerShapeRadius >= configData.windowH && cTransform.velocity.y > 0)
	//{
	//	cTransform.velocity.y = 0;
	//}

	//if ((cTransform.pos.x - configData.playerShapeRadius <= 0 && cTransform.velocity.x < 0) || (cTransform.pos.x + configData.playerShapeRadius >= configData.windowW && cTransform.velocity.x > 0))
	//{
	//	cTransform.velocity.x = 0;
	//}

	//cTransform.pos += cTransform.velocity * cTransform.speed;
}

//void MovementSystem::UpdateBulletMovement(Entity& bullet)
//{
//	CTransform& cTransform = bullet.GetComponent<CTransform>();
//	cTransform.pos += cTransform.velocity.Normalize() * cTransform.speed;
//}
//
//void MovementSystem::UpdateEnemyMovement(Entity& enemy)
//{
//	CTransform& cTransform = enemy.GetComponent<CTransform>();
//	cTransform.pos += cTransform.velocity.Normalize() * cTransform.speed;
//
//	//Bound Check (Change Later)
//	if (cTransform.pos.y - configData.enemyShapeRadius <= 0 || cTransform.pos.y + configData.enemyShapeRadius >= configData.windowH)
//	{
//		cTransform.velocity.y *= -1;
//	}
//
//	if (cTransform.pos.x - configData.enemyShapeRadius <= 0 || cTransform.pos.x + configData.enemyShapeRadius >= configData.windowW)
//	{
//		cTransform.velocity.x *= -1;
//	}
//}

//void MovementSystem::UpdateSmallEnemyMovement(Entity& smallEnemy)
//{
//	CTransform& cTransform = smallEnemy.GetComponent<CTransform>();
//	cTransform.pos += cTransform.velocity.Normalize() * cTransform.speed;
//
//	//Bound Check (Change Later)
//	if (cTransform.pos.y - configData.enemyShapeRadius <= 0 || cTransform.pos.y + configData.enemyShapeRadius >= configData.windowH)
//	{
//		cTransform.velocity.y *= -1;
//	}
//
//	if (cTransform.pos.x - configData.enemyShapeRadius <= 0 || cTransform.pos.x + configData.enemyShapeRadius >= configData.windowW)
//	{
//		cTransform.velocity.x *= -1;
//	}
//}

//void MovementSystem::ResetPlayer(Entity* player)
//{
//	playerID = player->Id();
//}
