#pragma once
#include "imgui.h"
#include "imgui-sfml.h"
#include "EntityManager.h"
#include "MyEvent.hpp"
#include "GridSystem.h"

class GUISystem
{
private:

	EntityManager& m_entityManager;
	GridSystem& m_gridSystem;
	size_t playerID;
	int& score;


public:

	MyEvent<void()> onResetGameBtnClick;

	GUISystem(EntityManager& entityManager ,int& score, GridSystem& gridSystem):m_entityManager(entityManager),score(score),m_gridSystem(gridSystem){}

	void HandleGUI();
	void ResetPlayer(Entity* player);
};

