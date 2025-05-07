#pragma once

#include "../Library/GameObject.h"
#include <vector>

class MapChip : public GameObject
{
private:
	std::vector<int> hImage_;

public:
	MapChip();
	~MapChip();

	void Update();
	void Draw();
};