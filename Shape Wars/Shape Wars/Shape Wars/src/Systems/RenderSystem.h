#pragma once
#include "GUISystem.h"
#include "EntityManager.hpp"
#include <cassert>

class RenderSystem
{
private:

	EntityManager& entityManager;
	size_t playerID;
	sf::RenderWindow& window;

	void RenderPlayer();
	void RenderBullets();
	void RenderSmallEnemies();
	void RenderEnemeis();
	void RenderGUI();

public:
	
	RenderSystem(EntityManager& entityManager, size_t playerID, sf::RenderWindow& window):entityManager(entityManager), playerID(playerID),window(window){}

	void HandleRenderSystem();
	void ResetPlayer(Entity* player);
};

