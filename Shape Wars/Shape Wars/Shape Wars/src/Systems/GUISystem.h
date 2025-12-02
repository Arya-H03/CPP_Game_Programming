#pragma once
#include "imgui.h"
#include "imgui-sfml.h"
#include "EntityManager.hpp"
#include "MyEvent.hpp"

class GUISystem
{
private:

	EntityManager& entityManager;
	size_t playerID;
	int& score;


public:

	MyEvent<void()> onResetGameBtnClick;

	GUISystem(EntityManager& entityManager, size_t playerID, int& score):entityManager(entityManager), playerID(playerID),score(score){}

	void HandleGUI();
	void ResetPlayer(Entity* player);
};

