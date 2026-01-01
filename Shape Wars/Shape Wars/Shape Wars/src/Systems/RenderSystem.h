#pragma once
#include "GUISystem.h"
#include "EntityManager.h"
#include "GridSystem.h"
#include <cassert>

class RenderSystem
{
private:

	EntityManager& m_entityManager;
	GridSystem& m_gridSystem;	
	sf::RenderWindow& window;
	
	void RenderPlayer();
	void RenderBullets();
	void RenderSmallEnemies();
	void RenderEnemeis();
	void RenderGUI();
	void RenderCells();

public:
	
	RenderSystem(EntityManager& entityManager,sf::RenderWindow& window,GridSystem& gridSystem);

	void HandleRenderSystem();
};

