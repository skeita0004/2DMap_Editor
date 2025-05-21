#pragma once
#include <cmath>

struct VECTOR2INT
{
	int x, y;
	VECTOR2INT() { x = 0; y = 0; }
	VECTOR2INT(int _x, int _y) { x = _x; y = _y; }
};

inline VECTOR2INT operator +(VECTOR2INT a, VECTOR2INT b)
{
	VECTOR2INT c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}

inline VECTOR2INT operator -(VECTOR2INT a, VECTOR2INT b)
{
	VECTOR2INT c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	return c;
}

inline VECTOR2INT operator /(VECTOR2INT a, int f)
{
	VECTOR2INT c;
	c.x = a.x / f;
	c.y = a.y / f;
	return c;
}

inline VECTOR2INT operator *(VECTOR2INT a, int f)
{
	VECTOR2INT c;
	c.x = a.x * f;
	c.y = a.y * f;
	return c;
}

inline float VSize(VECTOR2INT v)
{
	return sqrtf(v.x * v.x + v.y * v.y);
}