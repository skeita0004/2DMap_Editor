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


private:
	std::vector<int> hImage_;
	std::map<int, int> handleToIndex_;

	Vector2D<int> mousePosition_;
	bool isHold_;
	int heldIndex_;
	Vector2D<int> selected_;

	MapChipConfig toolAreaConf_;

	Vector2D<int> ScrollOffset_;

	Vector2D<int> GetViewOrigin() const;
	bool IsInChipArea(const Vector2D<int>& _mouse);

	/// @brief スクリーン座標をマップパレットのインデックスに変化する
	/// @param _mouse マウス座標
	/// @return インデックス
	Vector2D<int> ScreenToMapIndex(const Vector2D<int>& _mouse);
	
	/// @brief マップパレットのスクロール操作
	void PalletScroll();

	/// @brief マップチップの描画
	/// @param _x 選択場所ⅹ
	/// @param _y 選択場所y
	void DrawMapChip(const int _x, const int _y);

	/// @brief マップパレット領域の描画
	void DrawMapPallet();
};