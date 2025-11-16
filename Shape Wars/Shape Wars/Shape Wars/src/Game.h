#pragma once
#include<SFML/Graphics.hpp>
#include"EntityManager.hpp"
#include "imgui.h"
#include "imgui-sfml.h"
#include <SFML/Window.hpp>
#include "ConfigData.hpp"
#include "AudioData.hpp"
#include "MovementSystem.h"
#include "InputSystem.h"

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };


class Game
{
	sf::RenderWindow window;
	sf::Font font;
	sf::Clock clock;
	/*sf::Text m_text;*/

	PlayerConfig playerConfig;
	EnemyConfig enemyConfig;
	BulletConfig bulletConfig;
	ConfigData configData;
	AudioData audioData;

	EntityManager entities;
	MovementSystem movementSystem;
	InputSystem inputSystem;

	int score = 0;
	int currentFrame = 0;
	int lastEnemySpawnTime = 0;

	bool isGamePaused = false;
	bool isGameRunning = true;

	void init();
	void SetPaused(bool value);

	void SMovement();
	void SUserInput();
	void SLifeSpan();
	void SRender();
	void SGUI();
	void SEnemySpawner();
	void SCollision();
	void SResetGame();
	void SpawnPlayer();
	void SpawnEnemy();
	void SpawnSmallEnemies(std::shared_ptr<Entity> entity);

	void SpawnBullet(const Vec2f& mousePos);
	void CloseWindow();

	std::shared_ptr<Entity> Player();

public:

	Game(const std::string& config);

	void Run();

};


