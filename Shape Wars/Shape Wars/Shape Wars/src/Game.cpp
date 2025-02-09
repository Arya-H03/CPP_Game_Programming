#include "Game.h"
#include<iostream>

Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	m_fileData.ReadFromFile(path);
	m_window.create(sf::VideoMode({ m_fileData.windowW, m_fileData.windowH }), "Shape Wars");
	m_window.setFramerateLimit(m_fileData.frameLimit);

	ImGui::SFML::Init(m_window);
	ImGui::GetStyle().ScaleAllSizes(2.0f);
	ImGui::GetIO().FontGlobalScale = 2.0f;

	SpawnPlayer();
	

}

std::shared_ptr<Entity> Game::Player()
{
	auto& players = m_entities.GetEntities("Player");
	return players.front();
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
	auto entity = m_entities.AddEntity("Enemy");
	entity->Add<CTransform>(Vec2f(200, 200), Vec2f(1.0f, 1.0f), 0.0f);
	entity->Add<CShape>(32.0f, 8, sf::Color(255, 10, 10), sf::Color(255, 0, 0), 4.0f);
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
	auto& transform = Player()->Get<CTransform>();
	transform.pos.x += transform.velocity.x;
	transform.pos.y += transform.velocity.y;
}

void Game::SLifeSpan()
{

}

void Game::SUserInput()
{
	while (const auto event = m_window.pollEvent())
	{
		ImGui::SFML::ProcessEvent(m_window, *event);

		if (event->is<sf::Event::Closed>())
		{
			m_running = false;
			m_window.close();
		}

		/*if (event->is<sf::Event::KeyPressed>())
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:

			default:
				break;
			}
		}*/
	}

	
}

void Game::SRender()
{
	m_window.clear();

	Player()->Get<CShape>().circle.setPosition(Player()->Get<CTransform>().pos);
	Player()->Get<CTransform>().angle += 1.0f;
	Player()->Get<CShape>().circle.setRotation(sf::degrees(Player()->Get<CTransform>().angle));

	m_window.draw(Player()->Get<CShape>().circle);

	for (auto& enemy : m_entities.GetEntities("Enemy"))
	{
		enemy->Get<CShape>().circle.setPosition(enemy->Get<CTransform>().pos);
		enemy->Get<CTransform>().angle += 1.0f;
		enemy->Get<CShape>().circle.setRotation(sf::degrees(enemy->Get<CTransform>().angle));
		m_window.draw(enemy->Get<CShape>().circle);
	}

	ImGui::SFML::Render(m_window);

	m_window.display();


}

void Game::SGUI()
{
	ImGui::Begin("Shape Wars");
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Systems"))
		{
			ImGui::Text("This is the Systems tab!\nblah blah blah blah blah");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Entities"))
		{
			int btnId = 0;
			if (ImGui::CollapsingHeader("Entities by Tags"))
			{
				for (auto& [tag, entityVec] : m_entities.GetEntityMap())
				{
					std::string headerName = tag;
					if (ImGui::CollapsingHeader(headerName.c_str()))
					{
						bool isDestroyed = false;
						for (auto& e : entityVec)
						{
							//Delete btn
							static int clicked = 0;
							isDestroyed = false;
							sf::Color shapeColor = e->Get<CShape>().circle.getFillColor();
							ImGui::PushID(btnId);
							btnId++;
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(shapeColor.r / 255, shapeColor.g / 255, shapeColor.b / 255, 1.0f));
							if (ImGui::Button("D")) isDestroyed = true;
							ImGui::PopStyleColor();
							ImGui::PopID();
							ImGui::SameLine();
							//ID
							ImGui::Text("%i", e->Id());
							ImGui::SameLine();
							//Position
							ImGui::Text("(%.2f,%.2f)", e->Get<CTransform>().pos.x, e->Get<CTransform>().pos.y);

							if (isDestroyed) e->Destroy();
						}
					}
				}
			}

			if (ImGui::CollapsingHeader("All Entities"))
			{
				bool isDestroyed = false;
				for (auto& e : m_entities.GetEntities())
				{
					//Delete btn
					static int clicked = 0;
					isDestroyed = false;
					sf::Color shapeColor = e->Get<CShape>().circle.getFillColor();
					ImGui::PushID(btnId);
					btnId++;
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(shapeColor.r / 255, shapeColor.g / 255, shapeColor.b /255, 1.0f));
					if (ImGui::Button("D")) isDestroyed = true;
					ImGui::PopStyleColor();
					ImGui::PopID();
					ImGui::SameLine();
					//ID
					ImGui::Text("%i", e->Id());
					ImGui::SameLine();
					//Tag
					ImGui::Text("%s", e->Tag().c_str());
					ImGui::SameLine();
					//Position
					ImGui::Text("(%.2f,%.2f)", e->Get<CTransform>().pos.x, e->Get<CTransform>().pos.y);
					
					if (isDestroyed) e->Destroy();
				}
				
			}

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

void Game::SEnemySpawner()
{
	if (m_currentFrame - m_lastEnemySpawnTime >= 120)
	{
		SpawnEnemy();
		m_lastEnemySpawnTime = m_currentFrame;
	}
	
}

void Game::SCollision()
{

}

