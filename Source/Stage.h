#pragma once
#include "../Library/GameObject.h"
#include <vector>
#include "MapChip.hpp"
#include "MapEdit.hpp"
#include "MousePointer.hpp"

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
	MousePointer* mousePointer_;
};

