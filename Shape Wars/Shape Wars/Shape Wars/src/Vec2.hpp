#pragma once
#include<SFML/Graphics.hpp>
#include<math.h>

template<typename T>
class Vec2
{
public:

	T x = 0;
	T y = 0;

	Vec2() = default;

	Vec2(T x, T y) : this.x(x),this.y(y){ }

	Vec2(const sf::Vector2<T> &vec) :x(vec.x),y(vec.y) {}

	operator sf::Vector2<T>() { return sf::Vector2<T>(x, y); }

	Vec2 operator + (const Vec2& rhs) const {}

	Vec2 operator - (const Vec2& rhs) const {}

	Vec2 operator / (const Vec2& rhs) const {}

	Vec2 operator * (const Vec2& rhs) const {}

	bool operator == (const Vec2& rhs) const {}

	bool operator != (const Vec2& rhs) const {}

	void operator += (const Vec2& rhs){}

	void operator -= (const Vec2& rhs) {}

	void operator *= (const T val) {}

	void operator /= (const T rhs) {}

	float dist(const Vec2& rhs) const{}

};

using Vec2f = Vec2<float>;