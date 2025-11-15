#pragma once
#include "ConfigData.hpp"
#include "Entity.hpp"


class MovementSystem
{
private:
	const ConfigData& configData;
public:
	MovementSystem(const ConfigData& configData);

	void UpdatePlayerMovement(Entity& player);
	void UpdateBulletMovement(Entity& bullet);
	void UpdateEnemyMovement(Entity& enemy);
	void UpdateSmallEnemyMovement(Entity& smallEnemy);


};

