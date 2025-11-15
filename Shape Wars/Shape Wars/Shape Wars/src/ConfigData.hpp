#pragma once
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>

class ConfigData
{
private:
	void ReadFromFile(const std::string filepath)
	{
		std::string temp;
		std::ifstream fin(filepath);

		if (!fin)
		{
			std::cerr << "Could not open file: " << filepath;
		}

		while (fin >> temp)
		{
			if (temp == "Window")
			{
				if (!(fin >> windowW >> windowH >> frameLimit >> fullScreen))
				{
					std::cerr << "Could not read Window config";
					return;
				}
			}

			if (temp == "Font")
			{
				int r, g, b;
				if (!(fin >> fontFile >> fontSize >> r >> g >> b))
				{
					std::cerr << "Could not read Font config";
					return;
				}
				else fontColor = sf::Color(r, g, b);
			}

			if (temp == "Player")
			{
				int r1, g1, b1;
				int r2, g2, b2;
				if (!(fin >> playerShapeRadius >> playerCollisionRadius >> playerSpeed >> r1 >> g1 >> b1 >> r2 >> g2 >> b2 >> playerOutThickness >> playerShapeVer))
				{
					std::cerr << "Could not read Player config";
					return;
				}
				else
				{
					playerFillColor = sf::Color(r1, g1, b1);
					playerOutColor = sf::Color(r2, g2, b2);
				}
			}

			if (temp == "Enemy")
			{
				int r, g, b;
				if (!(fin >> enemyShapeRadius >> enemyCollisionRadius >> enemyMinSpeed >> enemyMaxSpeed >> r >> g >> b >> enemyOutThickness >> enemyMinShapeVer >> enemyMaxShapeVer >> smallEnemyLifeSpan >> spawnInterval))
				{
					std::cerr << "Could not read Enemy config";
					return;
				}
				else
				{
					enemyOutColor = sf::Color(r, g, b);
				}
			}

			if (temp == "Bullet")
			{
				int r1, g1, b1;
				int r2, g2, b2;
				if (!(fin >> bulletShapeRadius >> bulletCollisionRadius >> bulletSpeed >> r1 >> g1 >> b1 >> r2 >> g2 >> b2 >> bulletOutThickness >> bulletShapeVer >> bulletLifeSpan))
				{
					std::cerr << "Could not read Bullet config";
					return;
				}
				else
				{
					bulletFillColor = sf::Color(r1, g1, b1);
					bulletOutColor = sf::Color(r2, g2, b2);
				}
			}
		}
	}
public:

	//Window Data
	unsigned int windowH = 0;
	unsigned int windowW = 0;
	int frameLimit = 0;
	int fullScreen = 0; //0 is no 1 is yes

	//Font Data
	std::string fontFile = "";
	int fontSize = 0;
	sf::Color fontColor;

	//Player Data
	int playerShapeRadius = 0;
	int playerCollisionRadius = 0;
	float playerSpeed = 0;
	sf::Color playerFillColor;
	sf::Color playerOutColor;
	int playerOutThickness = 0;
	int playerShapeVer = 0;

	//Enemy Data
	int enemyShapeRadius = 0;
	int enemyCollisionRadius = 0;
	int enemyMaxSpeed = 0;
	int enemyMinSpeed = 0;
	sf::Color enemyOutColor;
	int enemyOutThickness = 0;
	int enemyMaxShapeVer = 0;
	int enemyMinShapeVer = 0;
	int smallEnemyLifeSpan = 0;
	int spawnInterval = 0;

	//Bullet Data
	int bulletShapeRadius = 0;
	int bulletCollisionRadius = 0;
	float bulletSpeed = 0;
	sf::Color bulletFillColor;
	sf::Color bulletOutColor;
	int bulletOutThickness = 0;
	int bulletShapeVer = 0;
	int bulletLifeSpan = 0;

	ConfigData(const std::string filepath)
	{
		ReadFromFile(filepath);
	}



};