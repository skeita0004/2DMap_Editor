#pragma once
#include "Vector2Int.h"

#include "../Library/GameObject.h"

class MousePointer : public GameObject
{
private:
	VECTOR2INT position_;

public:
	MousePointer();
	~MousePointer();

	void Update() override;
	void Draw()   override;

	VECTOR2INT GetPosition() { return position_; }
};