#pragma once

#include "../Library/GameObject.h"
#include "Screen.h"
#include <vector>



class MapEdit : public GameObject
{
private:
	//int *hImage_; //背景画像のハンドル配列へのポインタ
	std::vector<int> myMap_;

public:
	MapEdit();
	~MapEdit();

	void Update();
	void Draw();
};