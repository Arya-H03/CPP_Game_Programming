#pragma once
#include<SFML/Graphics.hpp>
#include<math.h>
#include<iostream>

template<typename T>
class Vec2
{
public:

	T x = 0;
	T y = 0;

	Vec2() = default;

	Vec2(T x, T y) : x(x), y(y) {}

	Vec2(const sf::Vector2<T>& vec) :x(vec.x), y(vec.y) {}

	operator sf::Vector2<T>() { return sf::Vector2<T>(x, y); }

	Vec2 operator + (const Vec2& rhs) const { return Vec2(x + rhs.x, y + rhs.y); }

	Vec2 operator - (const Vec2& rhs) const { return Vec2(x - rhs.x, y - rhs.y); }

	Vec2 operator / (const Vec2& rhs) const { return Vec2(x / rhs.x, y / rhs.y); }

	Vec2 operator * (const Vec2& rhs) const { return Vec2(x * rhs.x, y * rhs.y); }

	Vec2 operator / (const T value) const { return Vec2(x / value, y / value); }

	Vec2 operator * (const T value) const { return Vec2(x * value, y * value); }

	bool operator == (const Vec2& rhs) const { return(x == rhs.x && y == rhs.y); }

	bool operator != (const Vec2& rhs) const { return(x != rhs.x || y != rhs.y); }

	void operator += (const Vec2& rhs) { x += rhs.x; y += rhs.y; }

	void operator -= (const Vec2& rhs) { x -= rhs.x; y -= rhs.y; }

	void operator *= (const T val) { x *= val; y *= val; }

	void operator /= (const T val) { x /= val; y /= val; }

	float Dist(const Vec2& rhs) const { return std::sqrt((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y)); }

	void Print() { std::cerr << "Vec: (" << x << ", " << y << ")" << "\n"; }

	float Length() { return(std::sqrt(x * x + y * y)); }

	Vec2 Normalize()
	{
		if (Length() == 0) { return Vec2(0, 0); }

		return Vec2(x / Length(), y / Length());
	}

	static const Vec2<T> Zero;

};

using Vec2f = Vec2<float>;

template<typename T>
inline const Vec2<T> Vec2<T>::Zero = Vec2<T>(0, 0);