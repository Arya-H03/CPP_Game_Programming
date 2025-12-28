#pragma once
#include"EntityManager.hpp"

class Grid
{
private:

	size_t m_width;
	size_t m_height;
	size_t m_cellSize;
	size_t m_cellPerCol;
	size_t m_cellPerRow;
	float m_cellRadius;

	std::vector<std::vector<std::shared_ptr<Entity>>> cells;

	EntityManager& m_entityManager;
	
	void PopulateGrid();

	
public:

	Grid(size_t width, size_t height, size_t cellSize, EntityManager& entityManager);

	void DebugGrid();
};

