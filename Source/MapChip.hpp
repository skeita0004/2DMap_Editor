#pragma once

#include "../Library/GameObject.h"
#include <vector>
#include "Vector2.h"
#include "Vector2Int.h"

class MapChip : public GameObject
{
private:
	std::vector<int> hImage_;
	VECTOR2INT mousePosition_;
	bool isHold_;
	int holdedIndex_;
	VECTOR2 selected;

public:
	MapChip();
	~MapChip();

	void Update();
	void Draw();
	int  GetImageHandle();
};