#include "Game.h"
#include<iostream>


Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	m_configData.ReadFromFile(path);
	m_window.create(sf::VideoMode({ m_configData.windowW, m_configData.windowH }), "Shape Wars");
	m_window.setFramerateLimit(m_configData.frameLimit);

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
		SLifeSpan();
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
	auto player = m_entities.AddEntity("Player");
	player->Add<CTransform>(Vec2f(m_configData.windowW / 2, m_configData.windowH / 2), Vec2f(0, 0), 0.0f,m_configData.playerSpeed);
	player->Add<CShape>(m_configData.playerShapeRadius, m_configData.playerShapeVer, m_configData.playerFillColor, m_configData.playerOutColor, m_configData.playerOutThickness);
	player->Add<CInput>();
}

void Game::SpawnEnemy()
{
	//Random Pos
	int rXpos = m_configData.enemyShapeRadius + rand() % (m_configData.windowW - m_configData.enemyShapeRadius + 1);
	int rYpos = m_configData.enemyShapeRadius + rand() % (m_configData.windowH - m_configData.enemyShapeRadius + 1);
	
	//Random Vel
	int rXVel = -1 + rand() % 3;
	int rYVel = -1 + rand() % 3;
	while (rXVel == 0 && rYVel == 0)
	{
		rXVel = -1 + rand() % 3;
		rYVel = -1 + rand() % 3;
	}
	//Random Color
	uint32_t r, g, b;
	r = rand() % 256;
	g = rand() % 256;
	b = rand() % 256;

	//Random Ver
	int rVer = m_configData.enemyMinShapeVer + rand() % (m_configData .enemyMaxShapeVer +1);

	//Random Speed
	float rSpeed = m_configData.enemyMinSpeed + rand() % (m_configData.enemyMaxSpeed + 1);

	auto entity = m_entities.AddEntity("Enemy");
	entity->Add<CTransform>(Vec2f(rXpos, rYpos), Vec2f(rXVel, rYVel), 0, rSpeed);
	entity->Add<CShape>(m_configData.enemyShapeRadius, rVer, sf::Color(r, g, b), m_configData.enemyOutColor, 4.0f);
	entity->Add<CCollision>(m_configData.enemyCollisionRadius);
}

void Game::SpawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	auto& shape = entity->Get<CShape>();
	auto& transform = entity->Get<CTransform>();
	int ver = shape.circle.getPointCount();

	float a = 360 / ver;
	for (int i = 0; i < ver; i++)
	{
		float angle = ((90 - (a / 2)) - (a * i)) * (3.14159265358979323846 / 180.0);
		Vec2f vel(std::cos(angle) * shape.circle.getRadius(), std::sin(angle) * shape.circle.getRadius());

		auto sEnemy = m_entities.AddEntity("SmallEnemy");
		sEnemy->Add<CTransform>(transform.pos, vel,0, transform.speed);
		sEnemy->Add<CShape>(shape.circle.getRadius() / 2, shape.circle.getPointCount(), shape.circle.getFillColor(), shape.circle.getOutlineColor(), shape.circle.getOutlineThickness() / 2);
		sEnemy->Add<CCollision>(entity->Get<CCollision>().radius / 2);
		sEnemy->Add<CLifeSpan>(m_configData.smallEnemyLifeSpan);
	}

	

	
}

void Game::SpawnBullet(std::shared_ptr<Entity> player, const Vec2f& mousePos)
{
	auto bullet = m_entities.AddEntity("Bullet");
	Vec2f velocity = mousePos - player->Get<CTransform>().pos;
	bullet->Add<CTransform>(Player()->Get<CTransform>().pos, velocity, 0,m_configData.bulletSpeed);
	bullet->Add<CShape>(m_configData.bulletShapeRadius,m_configData.bulletShapeVer, m_configData.bulletFillColor, m_configData.bulletOutColor, m_configData.bulletOutThickness);
	bullet->Add<CCollision>(m_configData.bulletCollisionRadius);
	bullet->Add<CLifeSpan>(m_configData.bulletLifeSpan);
}
void Game::SpawnSpecialAbility(std::shared_ptr<Entity> entity, const Vec2f& mousePos)
{

}

void Game::SMovement()
{
	// Player Movement
	auto& transform = Player()->Get<CTransform>();
	auto& input = Player()->Get<CInput>();

	transform.velocity.y = (input.down - input.up);
	transform.velocity.x = (input.right - input.left);

	//Normalzie 
	if (transform.velocity.x != 0 && transform.velocity.y != 0) {transform.velocity = transform.velocity.Normalize();}

	//Bound Check
	if ((transform.pos.y - m_configData.playerShapeRadius <= 0 && transform.velocity.y < 0) || transform.pos.y + m_configData.playerShapeRadius >= m_configData.windowH && transform.velocity.y > 0)
	{
		transform.velocity.y = 0;
	}

	if ((transform.pos.x - m_configData.playerShapeRadius <= 0 && transform.velocity.x < 0) || (transform.pos.x + m_configData.playerShapeRadius >= m_configData.windowW && transform.velocity.x > 0))
	{
		transform.velocity.x = 0;
	}

	transform.pos += transform.velocity * transform.speed;


	//Bullet Movement
	for (auto& bullet : m_entities.GetEntities("Bullet"))
	{
		auto& transform = bullet->Get<CTransform>();
		transform.pos += transform.velocity.Normalize() * transform.speed;
	}

	//Enemy Movement
	for (auto& enemy : m_entities.GetEntities("Enemy"))
	{
		auto& transform = enemy->Get<CTransform>();
		transform.pos += transform.velocity.Normalize() * transform.speed;

		//Bound Check
		if (transform.pos.y - m_configData.enemyShapeRadius <= 0 || transform.pos.y + m_configData.enemyShapeRadius >= m_configData.windowH)
		{
			transform.velocity.y *= -1;
		}

		if (transform.pos.x - m_configData.enemyShapeRadius <= 0 || transform.pos.x + m_configData.enemyShapeRadius >= m_configData.windowW)
		{
			transform.velocity.x *= -1;
		}
	}

	//Move Small Enemy
	for (auto& sEnemy : m_entities.GetEntities("SmallEnemy"))
	{
		auto& transform = sEnemy->Get<CTransform>();
		transform.pos += transform.velocity.Normalize() * transform.speed;
	}
}

void Game::SLifeSpan()
{
	for (auto& bullet : m_entities.GetEntities("Bullet"))
	{
		bullet->Get<CLifeSpan>().remaining--;
		if (bullet->Get<CLifeSpan>().remaining <= 0) bullet->Destroy();
	}

	for (auto& sEnemy : m_entities.GetEntities("SmallEnemy"))
	{
		sEnemy->Get<CLifeSpan>().remaining--;
		if (sEnemy->Get<CLifeSpan>().remaining <= 0) sEnemy->Destroy();
	}
}

void Game::SUserInput()
{
	auto& inputC = Player()->Get<CInput>();

	while (const auto event = m_window.pollEvent())
	{
		
		ImGui::SFML::ProcessEvent(m_window, *event);

		if (event->is<sf::Event::Closed>())
		{
			m_running = false;
			m_window.close();
		}

		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			
			switch (keyPressed->scancode)
			{
			case sf::Keyboard::Scancode::W:
				inputC.up = true;				
				break;
			case sf::Keyboard::Scancode::S:
				inputC.down = true;
				break;
			case sf::Keyboard::Scancode::A:
				inputC.left = true;
				break;
			case sf::Keyboard::Scancode::D:
				inputC.right = true;
				break;
			default:
				break;
			}
		
		}

		else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			switch (keyReleased->scancode)
			{
			case sf::Keyboard::Scancode::W:
				inputC.up = false;
				break;
			case sf::Keyboard::Scancode::S:
				inputC.down = false;
				break;
			case sf::Keyboard::Scancode::A:
				inputC.left = false;
				break;
			case sf::Keyboard::Scancode::D:
				inputC.right = false;
				break;
			default:
				break;
			}

		}

		else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
		{
			if (mousePressed->button == sf::Mouse::Button::Left)
			{
				inputC.shoot = true;
				SpawnBullet(Player(), Vec2f(mousePressed->position.x, mousePressed->position.y));
			}
			
		}

		else if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouseReleased->button == sf::Mouse::Button::Left)
			{
				inputC.shoot = false;
			}
			
		}
	}

	
}

void Game::SRender()
{
	m_window.clear();

	Player()->Get<CShape>().circle.setPosition(Player()->Get<CTransform>().pos);
	Player()->Get<CTransform>().angle += 1.0f;
	Player()->Get<CShape>().circle.setRotation(sf::degrees(Player()->Get<CTransform>().angle));

	m_window.draw(Player()->Get<CShape>().circle);

	for (auto& bullet : m_entities.GetEntities("Bullet"))
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
		m_window.draw(shape.circle);
	}

	for (auto& enemy : m_entities.GetEntities("Enemy"))
	{
		enemy->Get<CShape>().circle.setPosition(enemy->Get<CTransform>().pos);
		enemy->Get<CTransform>().angle += 1.0f;
		enemy->Get<CShape>().circle.setRotation(sf::degrees(enemy->Get<CTransform>().angle));
		m_window.draw(enemy->Get<CShape>().circle);
	}

	for (auto& sEnemy : m_entities.GetEntities("SmallEnemy"))
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

		m_window.draw(sEnemy->Get<CShape>().circle);
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
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(static_cast<float>(shapeColor.r) / 255, static_cast<float>(shapeColor.g) / 255, static_cast<float>(shapeColor.b) / 255, 1.0f));
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
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(static_cast<float>(shapeColor.r)/255, static_cast<float>(shapeColor.g) / 255, static_cast<float>(shapeColor.b) / 255,1.0f));
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
	if (m_currentFrame - m_lastEnemySpawnTime >= m_configData.spawnInterval)
	{
		SpawnEnemy();
		m_lastEnemySpawnTime = m_currentFrame;
	}
	
}

void Game::SCollision()
{
	for (auto& bullet : m_entities.GetEntities("Bullet"))
	{
		auto& bTransform = bullet->Get<CTransform>();
		auto& bCollision = bullet->Get<CCollision>();
		for (auto & enemy : m_entities.GetEntities("Enemy"))
		{
			auto& eTransform = enemy->Get<CTransform>();
			auto& eCollision = enemy->Get<CCollision>();

			if (bTransform.pos.Dist(eTransform.pos) < abs(bCollision.radius - eCollision.radius))
			{
				bullet->Destroy();
				SpawnSmallEnemies(enemy);
				enemy->Destroy();

			}
		}
	}
}

