#include "MapEdit.hpp"
#include <cassert>
#include "Input.h"
#include "MapChip.hpp"

namespace
{
	constexpr int MAP_CHIP_NUM_WIDTH{ 20 };  // マップエディタ横方向の最大チップ数
	constexpr int MAP_CHIP_NUM_HEIGHT{ 20 }; // マップエディタ縦方向の最大チップ数
	constexpr int IMAGE_SIZE{ 32 }; // チップの画素数（正方形前提）
	constexpr int MAP_EDITOR_WIDTH{ MAP_CHIP_NUM_WIDTH * IMAGE_SIZE };
	constexpr int MAP_EDITOR_HEIGHT{ MAP_CHIP_NUM_HEIGHT * IMAGE_SIZE };
	constexpr int MAP_EDITOR_TOP_MARGIN{ (Screen::HEIGHT - MAP_CHIP_NUM_HEIGHT * IMAGE_SIZE) / 2 };
	constexpr int MAP_EDITOR_LEFT_MARGIN{ 240 };
}

MapEdit::MapEdit() :
	GameObject(),
	myMap_(MAP_CHIP_NUM_WIDTH* MAP_CHIP_NUM_HEIGHT, -1),
	mapEditRect_({ MAP_EDITOR_LEFT_MARGIN, MAP_EDITOR_TOP_MARGIN }, { MAP_EDITOR_WIDTH, MAP_EDITOR_HEIGHT }),
	isOnMapEdit_(false)
{
}

MapEdit::~MapEdit()
{	
}

void MapEdit::SetMap(VECTOR2INT _pos, int _value)
{
	assert(_pos.x >= 0 && _pos.x < MAP_CHIP_NUM_WIDTH);
	assert(_pos.y >= 0 && _pos.y < MAP_CHIP_NUM_HEIGHT);

	myMap_[_pos.y * MAP_CHIP_NUM_WIDTH + _pos.x] = _value;
}

int MapEdit::GetMap(VECTOR2INT _pos) const
{
	assert(_pos.x >= 0 && _pos.x < MAP_CHIP_NUM_WIDTH);
	assert(_pos.y >= 0 && _pos.y < MAP_CHIP_NUM_HEIGHT);
	return myMap_[_pos.y * MAP_CHIP_NUM_WIDTH + _pos.x];
}

void MapEdit::Update()
{
	GetMousePoint(&mousePosition_.x, &mousePosition_.y);

	selected.x = (mousePosition_.x - MAP_EDITOR_LEFT_MARGIN) / IMAGE_SIZE;
	selected.y = (mousePosition_.y - MAP_EDITOR_TOP_MARGIN) / IMAGE_SIZE;

	{
		int x = mousePosition_.x;
		int y = mousePosition_.y;

		int posX = mapEditRect_.position.x;
		int posY = mapEditRect_.position.y;
		int sizeX = mapEditRect_.imageSize.x;
		int sizeY = mapEditRect_.imageSize.y;

		if (x >= posX && y >= posY && x <= posX + sizeX && y <= posY + sizeY)
		{
			isOnMapEdit_ = true;
			if (Input::IsMouseDown(MOUSE_INPUT_LEFT))
			{
				MapChip* mapChip = FindGameObject<MapChip>();
				if (mapChip && mapChip->GetIsHold())
				{
					SetMap({ static_cast<int>(selected.x), static_cast<int>(selected.y) }, mapChip->GetHoldImage());
				}
			}
		}
		else
		{
			isOnMapEdit_ = false;
		}

	}
}

void MapEdit::Draw()
{

	{

		int posX = mapEditRect_.position.x;
		int posY = mapEditRect_.position.y;
		int sizeX = mapEditRect_.imageSize.x;
		int sizeY = mapEditRect_.imageSize.y;


		if (isOnMapEdit_)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
			//DrawBox(posX, posY, posX + sizeX, posY + sizeY, 0xff00ff, TRUE);
			DrawBox(MAP_EDITOR_LEFT_MARGIN + (selected.x * IMAGE_SIZE),
				MAP_EDITOR_TOP_MARGIN + (selected.y * IMAGE_SIZE),
				MAP_EDITOR_LEFT_MARGIN + (selected.x * IMAGE_SIZE + IMAGE_SIZE),
				MAP_EDITOR_TOP_MARGIN + (selected.y * IMAGE_SIZE + IMAGE_SIZE),
				0x00ffff, true, 2);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(posX, posY,
				posX + sizeX,
				posY + sizeY,
				0x11ffdf, FALSE, 3);


		for (int i = 1; i < MAP_EDITOR_WIDTH / IMAGE_SIZE; i++)
		{
			DrawLine(MAP_EDITOR_LEFT_MARGIN + i * IMAGE_SIZE, MAP_EDITOR_TOP_MARGIN,
						MAP_EDITOR_LEFT_MARGIN + i * IMAGE_SIZE, MAP_EDITOR_TOP_MARGIN + MAP_EDITOR_HEIGHT,
						0xffeedd, 1);
		}

		for (int i = 1; i < MAP_EDITOR_HEIGHT / IMAGE_SIZE; i++)
		{
			DrawLine(MAP_EDITOR_LEFT_MARGIN, MAP_EDITOR_TOP_MARGIN + i * IMAGE_SIZE,
					MAP_EDITOR_LEFT_MARGIN + MAP_EDITOR_WIDTH, MAP_EDITOR_TOP_MARGIN + i * IMAGE_SIZE,
						0xffeedd, 1);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		{
			int x = MAP_EDITOR_LEFT_MARGIN + (selected.x * IMAGE_SIZE);
			int y = MAP_EDITOR_TOP_MARGIN + (selected.y * IMAGE_SIZE);

			for (auto& myMap : myMap_)
			{
				DrawGraph(x, y, myMap, true);
			}

		}
	}
}
