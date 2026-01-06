#pragma once
#include"EntityManager.h"
#include"AssetManager.h"

class GridSystem
{
private:

	size_t m_width;
	size_t m_height;
	size_t m_cellSize;
	size_t m_cellPerCol;
	size_t m_cellPerRow;
	float m_cellRadius;
	bool m_canRenderCells = false;

	std::vector<std::vector<Entity>> cells;

	EntityManager& m_entityManager;
	
	void PopulateGrid();
	void AddDebugElementsForCell(Entity& cell);

	
public:

	GridSystem(size_t width, size_t height, size_t cellSize, EntityManager& entityManager);

	void ToggleCellsToRender();

};

