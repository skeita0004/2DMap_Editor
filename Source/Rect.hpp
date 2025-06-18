#pragma once
#include "Vector2Int.h"

struct Rect : public VECTOR2INT
{
	Rect(VECTOR2INT _position, VECTOR2INT _imageSize) :
		position(_position),
		imageSize(_imageSize)
	{}

	VECTOR2INT position;
	VECTOR2INT imageSize;
};