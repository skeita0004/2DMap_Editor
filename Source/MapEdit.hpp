#pragma once

#include "../Library/GameObject.h"
#include "Screen.h"
#include <vector>
#include "Vector2Int.h"
#include "Vector2.h"
#include "Rect.hpp"
#include <Windows.h>
#include "MapEditorConfig.hpp"

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
	void OpenMapData();
	void DeleteMapData();

private:
	//int *hImage_; //背景画像のハンドル配列へのポインタ

	/// @brief FillTile
	/// @param _hChoseImage   選んだ場所の画像ハンドル
	/// @param _hFillImage    塗りつぶしの画像
	/// @param _choseMapIndex 選んだ場所のインデックス
	void FillTile(const int& _hChoseImage, const int& _hFillImage, int _choseMapIndex);
	int ToSafeNeighbor(const int _from, const int _to);

	std::vector<int> myMap_;
	Rect mapEditRect_;
	bool isOnMapEdit_;
	VECTOR2INT mousePosition_;
	VECTOR2 selected;
	bool myMapIsEmpty_;
	bool canDelete_;
	int eraseIndex_;
	float deleteTimer_; // cu
	int hAlert_;
	int hDelMessage_;

	MapEditorConfig mec_;

};