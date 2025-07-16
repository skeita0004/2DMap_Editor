#pragma once

#include "../Library/GameObject.h"
#include <vector>
#include "Vector2.h"
#include "Vector2Int.h"
#include <map>
#include "MapChipConfig.hpp"
#include "../Library/Vector2D.hpp"

class MapChip : public GameObject
{
public:
	MapChip();
	~MapChip();

	void Update();
	void Draw();

	bool GetIsHold() { return isHold_; }
	int GetHoldImage();
	int GetChipIndex(int _handle);
	int GetImageHandle(int _Index);

	Vector2D<int> GetViewOrigin() const;
	bool IsInChipArea(const Vector2D<int>& _mouse);

	Vector2D<int> ScreenToMapIndex(const Vector2D<int>& _mouse);

private:
	void DrawMapChip(const int& _x, const int& _y);


	std::vector<int> hImage_;
	std::map<int, int> handleToIndex_;

	Vector2D<int> mousePosition_;
	bool isHold_;
	int heldIndex_;
	Vector2D<float> selected_;

	MapChipConfig mcg_;

	Vector2D<int> ScrollOffset_;

	void ToLocalPos( Vector2D<float>& _pos );
};