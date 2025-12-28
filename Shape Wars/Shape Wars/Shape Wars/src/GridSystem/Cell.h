#pragma once
#include "Vec2.hpp"
class Cell
{
private:
	 
	unsigned int size;
	Vec2<int> coord;
	Vec2<float> pos;


public:

	Cell(unsigned int size, Vec2<int> coord): size(size), coord(coord)
	{
		pos.x = 0.5f + (coord.x * size);
		pos.y = 0.5f + (coord.y * size);
	};

	unsigned int GetSize()
	{
		return size;
	}

	const Vec2<int>& GetCoord()
	{
		return coord;
	}

	const Vec2<float>& GetPos()
	{
		return pos;
	}
};

 