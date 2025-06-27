#include "MapEdit.hpp"
#include <cassert>
#include "Input.h"
#include "MapChip.hpp"
#include <string>
#include <fstream>

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
	//mapChip_ = FindGameObject<MapChip>();
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

		if (x > posX && y > posY && x < posX + sizeX && y < posY + sizeY)
		{
			MapChip* mapChip_ = FindGameObject<MapChip>();
			isOnMapEdit_ = true;
			if (Input::IsMouseHold(MOUSE_INPUT_LEFT))
			{
				if (Input::IsKeyHold(KEY_INPUT_F))
				{
					FillTile( mapChip_->GetHoldImage()));
				}
				if (mapChip_ && mapChip_->GetIsHold())
				{
					SetMap({ static_cast<int>(selected.x), static_cast<int>(selected.y) }, mapChip_->GetHoldImage());
				}
			}
			if (Input::IsMouseHold(MOUSE_INPUT_MIDDLE))
			{
				SetMap({ static_cast<int>(selected.x), static_cast<int>(selected.y) }, -1);
			}
		}
		else
		{
			isOnMapEdit_ = false;
		}
	}

	if (/*Input::IsKeyDown(KEY_INPUT_LCONTROL) && */Input::IsKeyDown(KEY_INPUT_S))
	{
		SaveMapData();
	}
}

void MapEdit::Draw()
{

	{

		int posX = mapEditRect_.position.x;
		int posY = mapEditRect_.position.y;
		int sizeX = mapEditRect_.imageSize.x;
		int sizeY = mapEditRect_.imageSize.y;

		for (int y = 0; y < MAP_CHIP_NUM_HEIGHT; y++)
		{
			for (int x = 0; x < MAP_CHIP_NUM_WIDTH; x++)
			{
				int hImage = GetMap({ x, y });
				if (hImage > -1)
				{
					DrawGraph(MAP_EDITOR_LEFT_MARGIN + x * IMAGE_SIZE,
						MAP_EDITOR_TOP_MARGIN + y * IMAGE_SIZE,
						hImage, TRUE);
				}
			}
		}

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
	}
}

void MapEdit::SaveMapData()
{
	int colCount = 0;
	printfDx("saved!");
	std::ofstream file("save.csv");
	

	file << "#HEAD, " << "\n";
	file << "WIDTH, " << MAP_CHIP_NUM_WIDTH << "," << "\n";
	file << "HEIGHT," << MAP_CHIP_NUM_HEIGHT << "," << "\n";
	file << ",\n";
	file << "#DATA, " << "\n";
	for (auto& mapData : myMap_)
	{
		colCount++;
		file << mapData << "," << " ";
		if (colCount == MAP_CHIP_NUM_WIDTH)
		{
			file << "\n";
			colCount = 0;
		}
	}
	file.close();
}

void MapEdit::FillTile(const int _hChoseImage, const int _hFillImage, const int _choseMapIndex)
{

	if (_choseMapIndex < 0 || myMap_.size() <= _choseMapIndex)
	{
		return;
	}

	if (myMap_[_choseMapIndex] != _hChoseImage
		|| myMap_[_choseMapIndex] == _hChoseImage)
	{
		return;
	}

	int upIndex = _choseMapIndex - MAP_CHIP_NUM_WIDTH;
	int downIndex = _choseMapIndex + MAP_CHIP_NUM_WIDTH;
	int leftIndex = _choseMapIndex - 1;
	int rightIndex = _choseMapIndex + 1;

	
	// forでできそう
	// 上方向
	FillTile(_hChoseImage, _hFillImage, upIndex);

	// 下方向
	FillTile(_hChoseImage, _hFillImage, downIndex);

	// 左方向
	FillTile(_hChoseImage, _hFillImage, leftIndex);

	// 右方向
	FillTile(_hChoseImage, _hFillImage, rightIndex);
}
