#pragma once
#include<SFML/Graphics.hpp>
#include"EntityManager.hpp"
#include "imgui.h"
#include "imgui-sfml.h"
#include <SFML/Window.hpp>
#include "FileData.hpp"

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig  { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN,SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };


class Game
{
	sf::RenderWindow m_window;
	EntityManager m_entities;
	sf::Font m_font;
	/*sf::Text m_text;*/
	PlayerConfig m_playerConfig;
	EnemyConfig m_enemyConfig;
	BulletConfig m_bulletConfig;
	FileData m_fileData;
	sf::Clock m_clock;
	int m_score = 0;
	int m_currentFrame = 0;
	int m_lastEnemySpawnTime = 0;
	bool m_paused = false;
	bool m_running = true;

	void init(const std::string& config);
	void SetPaused(bool value);

	void SMovement();
	void SUserInput();
	void SLifeSpan();
	void SRender();
	void SGUI();
	void SEnemySpawner();
	void SCollision();

	void SpawnPlayer();
	void SpawnEnemy();
	void SpawnSmallEnemies(std::shared_ptr<Entity> entity);
	void SpawnBullet(std::shared_ptr<Entity> entity, const Vec2f& mousePos);
	void SpawnSpecialAbility(std::shared_ptr<Entity> entity, const Vec2f& mousePos);

	std::shared_ptr<Entity> Player();

public:

	Game(const std::string& config);

	void Run();

};


