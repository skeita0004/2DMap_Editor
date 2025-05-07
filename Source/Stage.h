#pragma once
#include "../Library/GameObject.h"
#include <vector>
#include "MapChip.hpp"

class Stage : public GameObject
{
public:
	Stage();
	~Stage();

	void Init();
	void Update();
	void Draw();
	void Release();


private:
	//int *hImage_; //背景画像のハンドル配列へのポインタ
	std::vector<int> hImage_;
	MapChip* mapChip_;
};

