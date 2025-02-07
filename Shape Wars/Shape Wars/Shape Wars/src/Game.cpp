#include "Game.h"
#include<iostream>

Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	m_window.create(sf::VideoMode({ 1280, 720 }), "Shape Wars");
	m_window.setFramerateLimit(60);

	ImGui::SFML::Init(m_window);
	ImGui::GetStyle().ScaleAllSizes(2.0f);
	ImGui::GetIO().FontGlobalScale = 2.0f;

	//SpawnPlayer();

}

std::shared_ptr<Entity> Game::Player()
{
	/*auto& players = m_entities.GetEntities("player");
	return players.front();*/

	return nullptr;
}

void Game::Run()
{
	while (m_running)
	{
		m_entities.Update();

		ImGui::SFML::Update(m_window, m_clock.restart());

		SEnemySpawner();

		SMovement();
		SCollision();
		SUserInput();
		SGUI();
		SRender();

		m_currentFrame++;
	}
}

void Game::SetPaused(bool value)
{

}

void Game::SpawnPlayer()
{
	auto entity = m_entities.AddEntity("Player");
	entity->Add<CTransform>(Vec2f(200.0f, 200.0f), Vec2f(1.0f, 1.0f), 0.0f);
	entity->Add<CShape>(32.0f,8,sf::Color(10,10,10), sf::Color(255, 0, 0),4.0f);
	entity->Add<CInput>();
}

void Game::SpawnEnemy()
{
	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::SpawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	
}

void Game::SpawnBullet(std::shared_ptr<Entity> entity, const Vec2f& mousePos)
{

}
void Game::SpawnSpecialAbility(std::shared_ptr<Entity> entity, const Vec2f& mousePos)
{

}

void Game::SMovement()
{
	/*auto& transform = Player()->Get<CTransform>();
	transform.pos.x = transform.velocity.x;
	transform.pos.y = transform.velocity.y;*/
}

void Game::SLifeSpan()
{

}

void Game::SUserInput()
{
	//while (const auto event = m_window.pollEvent())
	//{
	//	ImGui::SFML::ProcessEvent(m_window, *event);

	//	if (event->is<sf::Event::Closed>())
	//	{
	//		m_running = false;
	//		//m_window.close();
	//	}

	//	/*if (event->is<sf::Event::KeyPressed>())
	//	{
	//		switch (event.key.code)
	//		{
	//		case sf::Keyboard::W:

	//		default:
	//			break;
	//		}
	//	}*/
	//}

	
}

void Game::SRender()
{
	m_window.clear();

	/*Player()->Get<CShape>().circle.setPosition(Player()->Get<CTransform>().pos);
	Player()->Get<CTransform>().angle += 1.0f;
	Player()->Get<CShape>().circle.setRotation(sf::degrees(Player()->Get<CTransform>().angle));

	m_window.draw(Player()->Get<CShape>().circle);*/

	ImGui::SFML::Render(m_window);

	m_window.display();


}

void Game::SGUI()
{
	ImGui::Begin("Shape Wars");
	ImGui::End();
}

void Game::SEnemySpawner()
{

}

void Game::SCollision()
{

}

