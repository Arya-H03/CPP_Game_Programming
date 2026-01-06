#pragma once
#include "Vec2.hpp"
#include <SFML/Graphics.hpp>
#include "TypeUtils.hpp"


class Component
{
public:

	bool exists = false;
};

class CTransform : public Component
{
public:

	Vec2f pos = { 0.0,0.0 };
	Vec2f velocity = { 0.0,0.0 };
	float angle = 0;
	float speed = 0;

	CTransform() = default;
	void Init(const Vec2f& p, const Vec2f& v, float a, float s)
	{
		pos = p;
		velocity = v;
		angle = a;
		speed = s;
	};
};

class CShape : public Component
{
public:

	sf::CircleShape circle;
	bool canRender = true;

	CShape() = default;
	void Init(float radius, size_t points, const sf::Color& fillColor, const sf::Color& outlineColor, float outlineThickness)
	{	
		circle.setRadius(radius);
		circle.setPointCount(points);
		circle.setFillColor(fillColor);
		circle.setOutlineColor(outlineColor);
		circle.setOutlineThickness(outlineThickness);
		circle.setOrigin({ radius, radius });
	}
};

class CCollision : public Component
{
public:

	float radius = 0;

	CCollision() = default;
	void Init(float radius)
	{
		this->radius = radius;
	}
};

class CScore : public Component
{
public:

	int score = 0;

	CScore() = default;
	void Init(int score)
	{
		this->score = score;
	}
};

class CLifeSpan : public Component
{
public:

	int lifeSpan = 0;
	int remaining = 0;

	CLifeSpan() = default;
	void Init(int totalLifeSpan)
	{
		this->lifeSpan = totalLifeSpan;
		this->remaining = totalLifeSpan;
	}
};

class CInput : public Component
{
public:

	bool up = false;
	bool down = false;
	bool right = false;
	bool left = false;
	bool shoot = false;

	CInput() = default;
};

class CCoord : public Component
{
public:

	Vec2<int> coord;

	CCoord() = default;
	void Init(const Vec2<int>& coord)
	{
		this->coord = coord;
	}
};

class CText : public Component
{
public:

	std::shared_ptr<sf::Text> cText;
	bool canRender = true;
	
	CText() = default;
	
	void Init(const sf::Font& textFont, const std::string& textContent, int textSize, const Vec2f& parentPos, const Vec2f& offsetFromParent, const sf::Color& textColor)
	{		
		cText = std::make_shared<sf::Text>(textFont, textContent, textSize);
		cText->setFillColor(textColor);
		cText->setPosition(parentPos + offsetFromParent);
	}
};

REGISTER_TYPE(CTransform)
REGISTER_TYPE(CShape)
REGISTER_TYPE(CCollision)
REGISTER_TYPE(CScore)
REGISTER_TYPE(CLifeSpan)
REGISTER_TYPE(CInput)
REGISTER_TYPE(CCoord)
REGISTER_TYPE(CText)
