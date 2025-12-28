#include "Grid.h"

Grid::Grid(size_t width, size_t height, size_t cellSize, EntityManager& entityManager) :m_width(width), m_height(height), m_cellSize(cellSize), m_entityManager(entityManager)
{
	m_cellPerCol = (int)(m_height / m_cellSize);
	m_cellPerRow = (int)(m_width / m_cellSize);
	m_cellRadius = std::sqrt(m_cellSize * m_cellSize + m_cellSize * m_cellSize) / 2;
	PopulateGrid();
}

void Grid::PopulateGrid()
{
	cells.resize(m_cellPerCol, std::vector<std::shared_ptr<Entity>>(m_cellPerRow));

	for (int i = 0; i < m_cellPerCol; ++i)
	{
		for (int j = 0; j < m_cellPerRow; ++j)
		{
			std::shared_ptr<Entity> cell = m_entityManager.AddEntity("Cell");
			cell->Add<CTransform>(Vec2f(i * m_cellSize, j * m_cellSize) + Vec2f(m_cellSize / 2, m_cellSize / 2), Vec2f(0, 0), 0, 0);

			uint32_t r, g, b;
			r = rand() % 256;
			g = rand() % 256;
			b = rand() % 256;

			cell->Add<CShape>(m_cellRadius,4, sf::Color(r, g, b),sf::Color(255,255,255), 1);
			cell->Get<CShape>().circle.setPosition(cell->Get<CTransform>().pos);
			cell->Get<CShape>().circle.setRotation(sf::degrees(45));

			cells[i][j] = cell;
		}
	}
}

void Grid::DebugGrid()
{

}
