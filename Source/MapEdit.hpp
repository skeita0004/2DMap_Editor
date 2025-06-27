#pragma once

#include "../Library/GameObject.h"
#include "Screen.h"
#include <vector>
#include "Vector2Int.h"
#include "Vector2.h"
#include "Rect.hpp"

class MapChip;

class MapEdit : public GameObject
{
public:
	MapEdit();
	~MapEdit();

	void SetMap(VECTOR2INT _pos, int _value);
	int GetMap(VECTOR2INT _pos) const;
	void Update();
	void Draw();
	bool IsOnMapEdit() { return isOnMapEdit_; }
	void SaveMapData();

	void FillTile(const int _hChoseImage, const int _hFillImage, const int _choseMapIndex);
private:
	//int *hImage_; //背景画像のハンドル配列へのポインタ

	std::vector<int> myMap_;
	Rect mapEditRect_;
	bool isOnMapEdit_;
	VECTOR2INT mousePosition_;
	VECTOR2 selected;

	//MapChip* mapChip_;
};