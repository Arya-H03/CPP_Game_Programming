#include "RenderSystem.h"

void RenderSystem::RenderPlayer()
{
	Entity* player = entityManager.FindEntityByID(playerID);

	if (player == nullptr) return;

	player->Get<CShape>().circle.setPosition(player->Get<CTransform>().pos);
	player->Get<CTransform>().angle += 1.0f;
	player->Get<CShape>().circle.setRotation(sf::degrees(player->Get<CTransform>().angle));

	window.draw(player->Get<CShape>().circle);
}

void RenderSystem::RenderBullets()
{
	for (auto& bullet : entityManager.GetEntities("Bullet"))
	{
		auto& shape = bullet->Get<CShape>();
		auto& lifeSpan = bullet->Get<CLifeSpan>();
		auto& transform = bullet->Get<CTransform>();

		if (lifeSpan.lifeSpan > 0)
		{
			uint8_t alpha = static_cast<uint8_t>((lifeSpan.remaining * 255) / lifeSpan.lifeSpan);
			sf::Color fillColor = shape.circle.getFillColor();
			sf::Color outColor = shape.circle.getOutlineColor();
			fillColor.a = alpha;
			outColor.a = alpha;
			shape.circle.setFillColor(fillColor);
			shape.circle.setOutlineColor(outColor);
		}

		shape.circle.setPosition(transform.pos);
		window.draw(shape.circle);
	}
}

void RenderSystem::RenderSmallEnemies()
{
	for (auto& sEnemy : entityManager.GetEntities("SmallEnemy"))
	{
		auto& lifeSpan = sEnemy->Get<CLifeSpan>();
		auto& transform = sEnemy->Get<CTransform>();
		auto& shape = sEnemy->Get<CShape>();

		if (lifeSpan.lifeSpan > 0)
		{
			uint8_t alpha = static_cast<uint8_t>((lifeSpan.remaining * 255) / lifeSpan.lifeSpan);
			sf::Color fillColor = shape.circle.getFillColor();
			sf::Color outColor = shape.circle.getOutlineColor();
			fillColor.a = alpha;
			outColor.a = alpha;
			shape.circle.setFillColor(fillColor);
			shape.circle.setOutlineColor(outColor);
		}

		transform.angle += 1.0f;
		shape.circle.setRotation(sf::degrees(sEnemy->Get<CTransform>().angle));
		shape.circle.setPosition(transform.pos);

		window.draw(sEnemy->Get<CShape>().circle);
	}
}

void RenderSystem::RenderEnemeis()
{
	for (auto& enemy : entityManager.GetEntities("Enemy"))
	{
		enemy->Get<CShape>().circle.setPosition(enemy->Get<CTransform>().pos);
		enemy->Get<CTransform>().angle += 1.0f;
		enemy->Get<CShape>().circle.setRotation(sf::degrees(enemy->Get<CTransform>().angle));
		window.draw(enemy->Get<CShape>().circle);
	}
}

void RenderSystem::RenderGUI()
{
	ImGui::SFML::Render(window);
}

void RenderSystem::RenderCells()
{
	for (auto& cell : entityManager.GetEntities("Cell"))
	{
		window.draw(cell->Get<CShape>().circle);
	}
}

void RenderSystem::HandleRenderSystem()
{
	window.clear();

	RenderPlayer();
	RenderBullets();
	RenderSmallEnemies();
	RenderEnemeis();
	RenderCells();
	RenderGUI();

	window.display();
}

void RenderSystem::ResetPlayer(Entity* player)
{
	playerID = player->Id();
}







