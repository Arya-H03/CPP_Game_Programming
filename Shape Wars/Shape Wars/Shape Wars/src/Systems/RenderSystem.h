#pragma once
#include "GUISystem.h"
#include "EntityManager.hpp"
#include "GridSystem.h"
#include <cassert>

class RenderSystem
{
private:

	EntityManager& m_entityManager;
	GridSystem& m_gridSystem;
	size_t playerID;
	sf::RenderWindow& window;
	/*sf::Font m_font; 
	sf::Text m_text;*/

	void RenderPlayer();
	void RenderBullets();
	void RenderSmallEnemies();
	void RenderEnemeis();
	void RenderGUI();
	void RenderCells();

public:
	
	RenderSystem(EntityManager& entityManager, size_t playerID, sf::RenderWindow& window,GridSystem& gridSystem);

	void HandleRenderSystem();
	void ResetPlayer(Entity* player);

};

