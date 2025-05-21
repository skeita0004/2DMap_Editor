#pragma once
#include <cmath>

struct VECTOR2 
{
	float x, y;
	VECTOR2() { x = 0; y = 0; }
	VECTOR2(float _x, float _y) { x = _x; y = _y; }
};

inline VECTOR2 operator +(VECTOR2 a, VECTOR2 b)
{
	VECTOR2 c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}

inline VECTOR2 operator -(VECTOR2 a, VECTOR2 b)
{
	VECTOR2 c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	return c;
}

inline VECTOR2 operator /(VECTOR2 a, float f)
{
	VECTOR2 c;
	c.x = a.x / f;
	c.y = a.y / f;
	return c;
}

inline VECTOR2 operator *(VECTOR2 a, float f)
{
	VECTOR2 c;
	c.x = a.x * f;
	c.y = a.y * f;
	return c;
}

inline float VSize(VECTOR2 v)
{
	return sqrtf(v.x * v.x + v.y * v.y);
}