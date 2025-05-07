#pragma once

#include "../Library/GameObject.h"

class MapEdit : public GameObject
{
private:
public:
	MapEdit();
	~MapEdit();

	void Update();
	void Draw();
};