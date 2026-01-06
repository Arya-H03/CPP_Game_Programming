#include "GridSystem.h"

GridSystem::GridSystem(size_t width, size_t height, size_t cellSize, EntityManager& entityManager) :m_width(width), m_height(height), m_cellSize(cellSize), m_entityManager(entityManager)
{
	m_cellPerCol = (int)(m_height / m_cellSize);
	m_cellPerRow = (int)(m_width / m_cellSize);
	m_cellRadius = std::sqrt(m_cellSize * m_cellSize + m_cellSize * m_cellSize) / 2;
	PopulateGrid();
}


void GridSystem::PopulateGrid()
{
	cells.resize(m_cellPerCol, std::vector<Entity>(m_cellPerRow));

	for (int i = 0; i < m_cellPerCol; ++i)
	{
		for (int j = 0; j < m_cellPerRow; ++j)
		{
			Entity cell = m_entityManager.AddEntity("Cell");
			CTransform& transform = cell.Add<CTransform>();
			transform.pos = Vec2f(i * m_cellSize, j * m_cellSize) + Vec2f(m_cellSize / 2, m_cellSize / 2);
			transform.velocity = Vec2f::Zero;
			transform.angle = 0;
			transform.speed = 0;

			CCoord& coord = cell.Add<CCoord>();
			coord.coord = Vec2<int>(i, j);
			cells[i][j] = cell;

			AddDebugElementsForCell(cell);
		}
	}

}

void GridSystem::AddDebugElementsForCell(Entity& cell)
{
	CCoord& cCoord = cell.GetComponent<CCoord>();
	CTransform& cTransform = cell.GetComponent<CTransform>();
	CText& text = cell.Add<CText>();
	text.Init(AssetManager::Instance().GetFont(), std::to_string(cCoord.coord.x) + ", " + std::to_string(cCoord.coord.y), 20, cTransform.pos, Vec2f(-m_cellRadius / 2, -m_cellRadius / 2), sf::Color::White);
	uint32_t r, g, b;
	r = rand() % 256;
	g = rand() % 256;
	b = rand() % 256;
	text.canRender = false;

	CShape& shape = cell.Add<CShape>();
	shape.Init(m_cellRadius, 4, sf::Color(r, g, b, 0), sf::Color(255, 255, 255), 1);
	shape.circle.setPosition(cell.GetComponent<CTransform>().pos);
	shape.circle.setRotation(sf::degrees(45));
	shape.canRender = false;
}


void GridSystem::ToggleCellsToRender()
{	
	bool newVal = !m_canRenderCells;
	for (int i = 0; i < m_cellPerCol; ++i)
	{
		for (int j = 0; j < m_cellPerRow; ++j)
		{
			size_t id = cells[i][j].Id();
			EntityMemoryPool::Instance().GetComponent<CShape>(id).canRender = newVal;
			EntityMemoryPool::Instance().GetComponent<CText>(id).canRender = newVal;
		}
	}
	m_canRenderCells = newVal;
}