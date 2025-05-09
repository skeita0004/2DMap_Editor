#pragma once
#include "../Library/GameObject.h"
#include <vector>
#include "MapChip.hpp"
#include "MapEdit.hpp"

class Stage : public GameObject
{
public:
	Stage();
	~Stage();

	void Update();
	void Draw();

private:
	MapChip* mapChip_;
	MapEdit* mapEdit_;
};

