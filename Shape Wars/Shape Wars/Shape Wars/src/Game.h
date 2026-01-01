#pragma once
#include<SFML/Graphics.hpp>
#include <cassert>
#include <SFML/Window.hpp>
#include "EntityManager.h"
#include "ConfigData.hpp"
#include "AudioData.hpp"
#include "MovementSystem.h"
#include "InputSystem.h"
#include "LifeSpan.h"
#include "CollisionSystem.h"
#include "GUISystem.h"
#include "RenderSystem.h"
#include "GridSystem.h"
#include "AssetManager.h"

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };


class Game
{
	sf::RenderWindow window;
	sf::Font font;
	sf::Clock clock;
	

	PlayerConfig playerConfig;
	EnemyConfig enemyConfig;
	BulletConfig bulletConfig;
	ConfigData configData;
	AudioData audioData;

	EntityManager entityManager; // declare first
	
	std::unique_ptr<CollisionSystem> collisionSystem;
	std::unique_ptr<MovementSystem> movementSystem;
	std::unique_ptr<GUISystem> guiSystem;
	std::unique_ptr<LifeSpanSystem> lifeSpanSystem;
	std::unique_ptr<InputSystem> inputSystem;
	std::unique_ptr<GridSystem> gridSystem;
	std::unique_ptr<RenderSystem> renderSystem;

	int score = 0;
	int currentFrame = 0;
	int lastEnemySpawnTime = 0;

	bool isGamePaused = false;
	bool isGameRunning = true;

	void init();
	void SetPaused(bool value);;
	void SEnemySpawner();
	void SResetGame();
	Entity SpawnPlayerEntity();
	void SpawnEnemy();
	void SpawnSmallEnemies(Entity& entity);
	void SpawnBullet(const Vec2f& mousePos);

	void ActionsOnPlayerHitEnemy(Entity& enemy);
	void ActionsOnBulletHitEnemy(Entity& bullet, Entity& enemy);
	void ActionsOnBulletHitSmallEnemy(Entity& bullet, Entity& enemy);

	void CloseWindow();

	std::optional<Entity> GetPlayerEntity();

public:

	Game(const std::string& config);

	void Run();

};


