#pragma once

#include "../Library/GameObject.h"
#include <vector>
#include "Vector2.h"
#include "Vector2Int.h"
#include <map>


class MapChip : public GameObject
{
public:
	MapChip();
	~MapChip();

	void Update();
	void Draw();
	bool GetIsHold() { return isHold_; }
	int GetHoldImage();
	//int GetHandle(int _index) { return hImage_[_index]; }
	int GetChipIndex(int _handle);
	int GetImageHandle(int _Index);

private:
	std::vector<int> hImage_;
	std::map<int, int> handleToIndex_;

	VECTOR2INT mousePosition_;
	bool isHold_;
	int heldIndex_;
	VECTOR2 selected_;
};