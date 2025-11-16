#include "Game.h"
#include<iostream>

Game::Game(const std::string& config) : configData(config), movementSystem(configData)
{
	init();
}

void Game::init()
{
	window.create(sf::VideoMode({ configData.windowW, configData.windowH }), "Shape Wars");
	window.setFramerateLimit(configData.frameLimit);

	ImGui::SFML::Init(window);
	ImGui::GetStyle().ScaleAllSizes(2.0f);
	ImGui::GetIO().FontGlobalScale = 2.0f;

	audioData.PlayMusic(10);

	inputSystem.AddEventToWindowClose([this]() {CloseWindow(); });
	inputSystem.AddEventToLeftClick([this](const Vec2f& mousePos) {SpawnBullet(mousePos); });

	SpawnPlayer();

	
}

std::shared_ptr<Entity> Game::Player()
{
	auto& players = entities.GetEntities("Player");
	if (players.size() <= 0) return nullptr;
	return players.front();
}

void Game::Run()
{
	while (isGameRunning)
	{
		entities.Update();
		ImGui::SFML::Update(window, clock.restart());

		SEnemySpawner();
		SMovement();
		SCollision();
		SUserInput();
		SLifeSpan();
		SGUI();
		SRender();

		currentFrame++;
	}
}

void Game::SetPaused(bool value)
{

}

void Game::SpawnPlayer()
{
	auto player = entities.AddEntity("Player");
	player->Add<CTransform>(Vec2f(configData.windowW / 2, configData.windowH / 2), Vec2f(0, 0), 0.0f, configData.playerSpeed);
	player->Add<CShape>(configData.playerShapeRadius, configData.playerShapeVer, configData.playerFillColor, configData.playerOutColor, configData.playerOutThickness);
	player->Add<CInput>();
}

void Game::SpawnEnemy()
{
	//Random Pos
	int rXpos = configData.enemyShapeRadius + rand() % (configData.windowW - configData.enemyShapeRadius + 1);
	int rYpos = configData.enemyShapeRadius + rand() % (configData.windowH - configData.enemyShapeRadius + 1);

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
	int rVer = configData.enemyMinShapeVer + rand() % (configData.enemyMaxShapeVer + 1);

	//Random Speed
	float rSpeed = configData.enemyMinSpeed + rand() % (configData.enemyMaxSpeed + 1);

	auto entity = entities.AddEntity("Enemy");
	entity->Add<CTransform>(Vec2f(rXpos, rYpos), Vec2f(rXVel, rYVel), 0, rSpeed);
	entity->Add<CShape>(configData.enemyShapeRadius, rVer, sf::Color(r, g, b), configData.enemyOutColor, 4.0f);
	entity->Add<CCollision>(configData.enemyCollisionRadius);
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

		auto sEnemy = entities.AddEntity("SmallEnemy");
		sEnemy->Add<CTransform>(transform.pos, vel, 0, transform.speed);
		sEnemy->Add<CShape>(shape.circle.getRadius() / 2, shape.circle.getPointCount(), shape.circle.getFillColor(), shape.circle.getOutlineColor(), shape.circle.getOutlineThickness() / 2);
		sEnemy->Add<CCollision>(entity->Get<CCollision>().radius / 2);
		sEnemy->Add<CLifeSpan>(configData.smallEnemyLifeSpan);
	}




}

void Game::SpawnBullet(const Vec2f& mousePos)
{
	auto player = Player();
	auto bullet = entities.AddEntity("Bullet");
	Vec2f velocity = mousePos - player->Get<CTransform>().pos;
	bullet->Add<CTransform>(Player()->Get<CTransform>().pos, velocity, 0, configData.bulletSpeed);
	bullet->Add<CShape>(configData.bulletShapeRadius, configData.bulletShapeVer, configData.bulletFillColor, configData.bulletOutColor, configData.bulletOutThickness);
	bullet->Add<CCollision>(configData.bulletCollisionRadius);
	bullet->Add<CLifeSpan>(configData.bulletLifeSpan);

	audioData.PlayShootSFX(200);
}
void Game::CloseWindow()
{
	isGameRunning = false;
	window.close();
}

void Game::SMovement()
{
	std::shared_ptr<Entity> player = Player();
	if (player) movementSystem.UpdatePlayerMovement(*player);

	for (auto& bullet : entities.GetEntities("Bullet"))
	{
		movementSystem.UpdateBulletMovement(*bullet);
	}


	for (auto& enemy : entities.GetEntities("Enemy"))
	{
		movementSystem.UpdateEnemyMovement(*enemy);
	}


	for (auto& smallEnemy : entities.GetEntities("SmallEnemy"))
	{
		movementSystem.UpdateSmallEnemyMovement(*smallEnemy);
	}

}

void Game::SLifeSpan()
{
	for (auto& bullet : entities.GetEntities("Bullet"))
	{
		bullet->Get<CLifeSpan>().remaining--;
		if (bullet->Get<CLifeSpan>().remaining <= 0) bullet->Destroy();
	}

	for (auto& sEnemy : entities.GetEntities("SmallEnemy"))
	{
		sEnemy->Get<CLifeSpan>().remaining--;
		if (sEnemy->Get<CLifeSpan>().remaining <= 0) sEnemy->Destroy();
	}
}

void Game::SUserInput()
{
	if (!Player()) return;

	inputSystem.HandleInput(window, Player()->Get<CInput>());
}

void Game::SRender()
{
	window.clear();
	if (Player())
	{
		Player()->Get<CShape>().circle.setPosition(Player()->Get<CTransform>().pos);
		Player()->Get<CTransform>().angle += 1.0f;
		Player()->Get<CShape>().circle.setRotation(sf::degrees(Player()->Get<CTransform>().angle));

		window.draw(Player()->Get<CShape>().circle);
	}

	for (auto& bullet : entities.GetEntities("Bullet"))
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

	for (auto& enemy : entities.GetEntities("Enemy"))
	{
		enemy->Get<CShape>().circle.setPosition(enemy->Get<CTransform>().pos);
		enemy->Get<CTransform>().angle += 1.0f;
		enemy->Get<CShape>().circle.setRotation(sf::degrees(enemy->Get<CTransform>().angle));
		window.draw(enemy->Get<CShape>().circle);
	}

	for (auto& sEnemy : entities.GetEntities("SmallEnemy"))
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

	ImGui::SFML::Render(window);
	window.display();
}

void Game::SGUI()
{
	ImGui::Begin("Shape Wars");
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Game"))
		{

			ImGui::Text("%s%i", "Score:", score);
			int clicked = 0;
			if (ImGui::Button("Replay")) clicked++;

			if (clicked & 1)
			{
				if (Player()) Player()->Destroy();
				SResetGame();
				clicked = 0;
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Entities"))
		{
			int btnId = 0;
			if (ImGui::CollapsingHeader("Entities by Tags"))
			{
				for (auto& [tag, entityVec] : entities.GetEntityMap())
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
				for (auto& e : entities.GetEntities())
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
	if (currentFrame - lastEnemySpawnTime >= configData.spawnInterval)
	{
		SpawnEnemy();
		lastEnemySpawnTime = currentFrame;
	}

}

void Game::SCollision()
{
	for (auto& bullet : entities.GetEntities("Bullet"))
	{
		auto& bTransform = bullet->Get<CTransform>();
		auto& bCollision = bullet->Get<CCollision>();
		for (auto& enemy : entities.GetEntities("Enemy"))
		{
			auto& eTransform = enemy->Get<CTransform>();
			auto& eCollision = enemy->Get<CCollision>();

			if (bTransform.pos.Dist(eTransform.pos) < abs(bCollision.radius - eCollision.radius))
			{
				bullet->Destroy();
				SpawnSmallEnemies(enemy);
				enemy->Destroy();
				audioData.PlayHitSFX(10);
				score++;
			}
		}

		for (auto& enemy : entities.GetEntities("SmallEnemy"))
		{
			auto& eTransform = enemy->Get<CTransform>();
			auto& eCollision = enemy->Get<CCollision>();

			if (bTransform.pos.Dist(eTransform.pos) < abs(bCollision.radius - eCollision.radius))
			{
				bullet->Destroy();
				enemy->Destroy();
				audioData.PlayHitSFX(10);
				score++;

			}
		}
	}

	if (Player())
	{
		auto& pTransform = Player()->Get<CTransform>();
		auto& pCollision = Player()->Get<CCollision>();

		for (auto& enemy : entities.GetEntities("Enemy"))
		{
			auto& eTransform = enemy->Get<CTransform>();
			auto& eCollision = enemy->Get<CCollision>();

			if (pTransform.pos.Dist(eTransform.pos) < abs(pCollision.radius - eCollision.radius))
			{

				enemy->Destroy();
				Player()->Destroy();
				audioData.PlayDeathSFX(15);

			}
		}

		for (auto& smallEnemy : entities.GetEntities("SmallEnemy"))
		{
			auto& eTransform = smallEnemy->Get<CTransform>();
			auto& eCollision = smallEnemy->Get<CCollision>();

			if (pTransform.pos.Dist(eTransform.pos) < abs(pCollision.radius - eCollision.radius))
			{

				smallEnemy->Destroy();
				Player()->Destroy();
				audioData.PlayDeathSFX(15);

			}
		}
	}

}

void Game::SResetGame()
{
	for (auto& entity : entities.GetEntities())
	{
		entity->Destroy();
	}
	score = 0;
	SpawnPlayer();
}