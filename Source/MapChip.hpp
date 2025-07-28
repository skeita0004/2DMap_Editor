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

	/// @brief �X�N���[�����W���}�b�v�p���b�g�̃C���f�b�N�X�ɕω�����
	/// @param _mouse �}�E�X���W
	/// @return �C���f�b�N�X
	Vector2D<int> ScreenToMapIndex(const Vector2D<int>& _mouse);
	
	/// @brief �}�b�v�p���b�g�̃X�N���[������
	void PalletScroll();

	/// @brief �}�b�v�`�b�v�̕`��
	/// @param _x �I���ꏊ�I
	/// @param _y �I���ꏊy
	void DrawMapChip(const int _x, const int _y);

	/// @brief �}�b�v�p���b�g�̈�̕`��
	void DrawMapPallet();
};