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

	static inline const ImVec4 idColor {225,0,0,225};
	static inline const ImVec4 addressColor {0,0,255,225};
	static inline const ImVec4 sizeColor {255,255,0,225};

	void DrawEntitiesTab();
	void DrawComponentsTab();

public:

	MyEvent<void()> onResetGameBtnClick;

	GUISystem(EntityManager& entityManager ,int& score, GridSystem& gridSystem):m_entityManager(entityManager),score(score),m_gridSystem(gridSystem){}

	void HandleGUI();
};

