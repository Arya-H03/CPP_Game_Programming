#include "Game.h"
#include<iostream>

Game::Game(const std::string& config) : configData(config)
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

	SpawnPlayer();

	collisionSystem = std::make_unique<CollisionSystem>(entityManager, Player()->Id());
	movementSystem = std::make_unique<MovementSystem>(configData, entityManager, Player()->Id());
	guiSystem = std::make_unique<GUISystem>(entityManager, Player()->Id(),score);
	lifeSpanSystem = std::make_unique<LifeSpanSystem>(entityManager);
	inputSystem = std::make_unique<InputSystem>(window, entityManager, Player()->Id());
	renderSystem = std::make_unique<RenderSystem>(entityManager, Player()->Id(), window);

	collisionSystem->onPlayerCollisionWithEnemies.Subscribe<Game, &Game::ActionsOnPlayerHitEnemy>(this);
	collisionSystem->onBulletCollisionWithEnemies.Subscribe<Game, &Game::ActionsOnBulletHitEnemy>(this);
	collisionSystem->onBulletCollisionWithSmallEnemy.Subscribe<Game, &Game::ActionsOnBulletHitSmallEnemy>(this);

	guiSystem->onResetGameBtnClick.Subscribe<Game, &Game::SResetGame>(this);

	inputSystem->onWindowClose.Subscribe<Game, &Game::CloseWindow>(this);
	inputSystem->onLeftClick.Subscribe<Game, &Game::SpawnBullet>(this);

	audioData.PlayMusic(10);
}

Entity* Game::Player()
{
	auto& players = entityManager.GetEntities("Player");
	if (players.size() <= 0) return nullptr;
	return players.front().get();
}

void Game::Run()
{
	while (isGameRunning)
	{
		ImGui::SFML::Update(window, clock.restart());
		entityManager.Update();

		//Input
		inputSystem->HandleInput();

		//Game Logic
		movementSystem->HandleMovementSystem();
		lifeSpanSystem->HandleLifeSpanSystem();
		collisionSystem->HandleCollisionSystem();
		SEnemySpawner();
		guiSystem->HandleGUI();

		//Render
		renderSystem->HandleRenderSystem();

		currentFrame++;
	}
}

void Game::SetPaused(bool value)
{

}

Entity* Game::SpawnPlayer()
{
	auto player = entityManager.AddEntity("Player");
	player->Add<CTransform>(Vec2f(configData.windowW / 2, configData.windowH / 2), Vec2f(0, 0), 0.0f, configData.playerSpeed);
	player->Add<CShape>(configData.playerShapeRadius, configData.playerShapeVer, configData.playerFillColor, configData.playerOutColor, configData.playerOutThickness);
	player->Add<CInput>();

	return player.get();
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

	auto entity = entityManager.AddEntity("Enemy");
	entity->Add<CTransform>(Vec2f(rXpos, rYpos), Vec2f(rXVel, rYVel), 0, rSpeed);
	entity->Add<CShape>(configData.enemyShapeRadius, rVer, sf::Color(r, g, b), configData.enemyOutColor, 4.0f);
	entity->Add<CCollision>(configData.enemyCollisionRadius);
}

void Game::SpawnSmallEnemies(Entity& entity)
{
	auto& shape = entity.Get<CShape>();
	auto& transform = entity.Get<CTransform>();
	int ver = shape.circle.getPointCount();

	float a = 360 / ver;
	for (int i = 0; i < ver; i++)
	{
		float angle = ((90 - (a / 2)) - (a * i)) * (3.14159265358979323846 / 180.0);
		Vec2f vel(std::cos(angle) * shape.circle.getRadius(), std::sin(angle) * shape.circle.getRadius());

		auto sEnemy = entityManager.AddEntity("SmallEnemy");
		sEnemy->Add<CTransform>(transform.pos, vel, 0, transform.speed);
		sEnemy->Add<CShape>(shape.circle.getRadius() / 2, shape.circle.getPointCount(), shape.circle.getFillColor(), shape.circle.getOutlineColor(), shape.circle.getOutlineThickness() / 2);
		sEnemy->Add<CCollision>(entity.Get<CCollision>().radius / 2);
		sEnemy->Add<CLifeSpan>(configData.smallEnemyLifeSpan);
	}
}

void Game::SpawnBullet(const Vec2f& mousePos)
{
	auto player = Player();
	if (!player) return;

	auto bullet = entityManager.AddEntity("Bullet");
	Vec2f velocity = mousePos - player->Get<CTransform>().pos;
	bullet->Add<CTransform>(Player()->Get<CTransform>().pos, velocity, 0, configData.bulletSpeed);
	bullet->Add<CShape>(configData.bulletShapeRadius, configData.bulletShapeVer, configData.bulletFillColor, configData.bulletOutColor, configData.bulletOutThickness);
	bullet->Add<CCollision>(configData.bulletCollisionRadius);
	bullet->Add<CLifeSpan>(configData.bulletLifeSpan);

	audioData.PlayShootSFX(200);
}
void Game::ActionsOnPlayerHitEnemy(Entity& enemy)
{
	enemy.Destroy();
	Player()->Destroy();
	audioData.PlayDeathSFX(15);
}

void Game::ActionsOnBulletHitEnemy(Entity& bullet, Entity& enemy)
{
	bullet.Destroy();
	SpawnSmallEnemies(enemy);
	enemy.Destroy();
	audioData.PlayHitSFX(10);
	score++;
}

void Game::ActionsOnBulletHitSmallEnemy(Entity& bullet, Entity& enemy)
{
	bullet.Destroy();
	enemy.Destroy();
	audioData.PlayHitSFX(10);
	score++;
}

void Game::CloseWindow()
{
	isGameRunning = false;
	window.close();
}

void Game::SEnemySpawner()
{
	if (currentFrame - lastEnemySpawnTime >= configData.spawnInterval)
	{
		SpawnEnemy();
		lastEnemySpawnTime = currentFrame;
	}

}

void Game::SResetGame()
{
	for (auto& entity : entityManager.GetEntities())
	{
		entity->Destroy();
	}
	Entity* newPlayer = SpawnPlayer();

	score = 0;
	collisionSystem->ResetPlayer(newPlayer);
	movementSystem->ResetPlayer(newPlayer);
	guiSystem->ResetPlayer(newPlayer);
	inputSystem->ResetPlayer(newPlayer);
	renderSystem->ResetPlayer(newPlayer);
}