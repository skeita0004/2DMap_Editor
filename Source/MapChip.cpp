#define NOMINMAX

#include "MapChip.hpp"
#include "Screen.h"
#include "Input.h"
#include <map>
#include "../Library/MyDxLib.hpp"
#include <algorithm>

// コードをより良いものにしようのコーナー

MapChip::MapChip() :
	GameObject(),
	hImage_(std::vector<int>(0, -1)),
	handleToIndex_(),
	mousePosition_({0, 0}),
	isHold_(false),
	heldIndex_(-1),
	selected_({0, 0}),
	toolAreaConf_(GetMapChipConfig()),
	ScrollOffset_({0, 0})
{
	hImage_ = std::vector<int>(toolAreaConf_.IMAGE_TILE_NUM.x * toolAreaConf_.IMAGE_TILE_NUM.y, -1);

	LoadDivGraph("Assets/img/bg.png",
		toolAreaConf_.IMAGE_TILE_NUM.x * toolAreaConf_.IMAGE_TILE_NUM.y,
		toolAreaConf_.IMAGE_TILE_NUM.x, toolAreaConf_.IMAGE_TILE_NUM.y,
		toolAreaConf_.IMAGE_PX_SIZE, toolAreaConf_.IMAGE_PX_SIZE,
		hImage_.data());

	handleToIndex_.clear();

	// ハンドルとインデックスの紐づけ
	for (int i = 0; i < hImage_.size(); i++)
	{
		handleToIndex_.insert(std::make_pair(hImage_[i], i));
	}
}

MapChip::~MapChip()
{
	for (auto& image : hImage_)
	{
		if (image != -1)
		{
			DeleteGraph(image);
			image = -1;
		}
	}
	hImage_.clear();
}

void MapChip::Update()
{
	GetMousePoint(&mousePosition_.x, &mousePosition_.y);
	selected_ = ScreenToMapIndex(mousePosition_);

	if (IsInChipArea(mousePosition_))
	{
		PalletScroll();
		if (Input::IsMouseDown(MOUSE_INPUT_LEFT))
		{
			isHold_ = true;
			heldIndex_ = selected_.x + std::min(ScrollOffset_.x, toolAreaConf_.IMAGE_TILE_NUM.x - 1) + std::min(selected_.y + abs(ScrollOffset_.y), toolAreaConf_.IMAGE_TILE_NUM.y - 1) * toolAreaConf_.IMAGE_TILE_NUM.x;
		}
	}

	if (Input::IsMouseDown(MOUSE_INPUT_RIGHT))
	{
		heldIndex_ = -1;
	}
}

void MapChip::Draw()
{
	DrawMapPallet();

	// 選択中のマップチップの描画
	if (isHold_ && heldIndex_ != -1)
	{
		MyDxLib::DrawExtendGraph(mousePosition_,
								 { toolAreaConf_.IMAGE_PX_SIZE, toolAreaConf_.IMAGE_PX_SIZE },
								 hImage_[heldIndex_],
								 true);
	}
}

int MapChip::GetHoldImage()
{
	return hImage_[heldIndex_];
}

int MapChip::GetChipIndex(int _handle)
{
	if (_handle != -1)
	{
		return handleToIndex_[_handle];
	}
	else
	{
		return -1;
	}
}

int MapChip::GetImageHandle(int _Index)
{
	if (_Index < 0)
	{
		return -1;
	}
	return hImage_[_Index];
}

Vector2D<int> MapChip::GetViewOrigin() const
{
	return Vector2D<int>();
}

bool MapChip::IsInChipArea(const Vector2D<int>& _mouse)
{
	return (_mouse.x >= toolAreaConf_.TOP_LEFT.x && _mouse.y >= toolAreaConf_.TOP_LEFT.y && _mouse.x <= toolAreaConf_.BOTTOM_RIGHT.x && _mouse.y <= toolAreaConf_.BOTTOM_RIGHT.y);
}

Vector2D<int> MapChip::ScreenToMapIndex(const Vector2D<int>& _mouse)
{
	return { (_mouse.x - (Screen::WIDTH - toolAreaConf_.MAP_WINDOW_SIZE.x)) / toolAreaConf_.IMAGE_PX_SIZE,
			  _mouse.y / toolAreaConf_.IMAGE_PX_SIZE };
}

void MapChip::DrawMapChip(const int _x, const int _y)
{
	int x = _x;
	int y = _y;
	
	int index = std::min(x + ScrollOffset_.x, toolAreaConf_.IMAGE_TILE_NUM.x - 1) + std::min(y + abs(ScrollOffset_.y), toolAreaConf_.IMAGE_TILE_NUM.y - 1) * toolAreaConf_.IMAGE_TILE_NUM.x;
	
	DrawGraph(toolAreaConf_.TOP_LEFT.x + x * toolAreaConf_.IMAGE_PX_SIZE,
			 (toolAreaConf_.TOP_LEFT.y + y ) * toolAreaConf_.IMAGE_PX_SIZE,
			 hImage_[index], TRUE );

	int verticalLineStartX = toolAreaConf_.TOP_LEFT.x + toolAreaConf_.IMAGE_PX_SIZE * x;
	int horizontalLineStartY = toolAreaConf_.TOP_LEFT.y + toolAreaConf_.IMAGE_PX_SIZE * y;

	// Vertical Grid
	DrawLine(verticalLineStartX,
			 toolAreaConf_.TOP_LEFT.y,
			 verticalLineStartX,
			 toolAreaConf_.TOP_LEFT.y + toolAreaConf_.MAP_WINDOW_SIZE.y,
			 0xffffff );

	// Horizontal Grid
	DrawLine(toolAreaConf_.TOP_LEFT.x,
			 horizontalLineStartY,
			 toolAreaConf_.TOP_LEFT.x + toolAreaConf_.MAP_WINDOW_SIZE.x,
			 horizontalLineStartY,
			 0xffffff);
}

void MapChip::PalletScroll()
{
	float mouseRot = GetMouseWheelRotVolF();
	
	bool mouseWheelFrontRot = mouseRot > 0;
	bool mouseWheelBackRot = mouseRot < 0;
	
	bool horizontalScrollTrigger = Input::IsKeyHold(KEY_INPUT_LSHIFT) || Input::IsKeyHold(KEY_INPUT_RSHIFT);

	if (horizontalScrollTrigger)
	{
		if (Input::IsKeyDown(KEY_INPUT_LEFT) || mouseWheelFrontRot)
		{
			ScrollOffset_.x = std::max(0, ScrollOffset_.x - 1);
		}
		if (Input::IsKeyDown(KEY_INPUT_RIGHT) || mouseWheelBackRot)
		{
			ScrollOffset_.x = std::min(toolAreaConf_.MAP_CHIP_NUM.x, ScrollOffset_.x + 1); // 一行で済む
		}
	}
	else
	{
		if (Input::IsKeyDown(KEY_INPUT_UP) || mouseWheelFrontRot)
		{
			ScrollOffset_.y += 1;
			ScrollOffset_.y = std::clamp<int>(ScrollOffset_.y, -toolAreaConf_.MAP_CHIP_NUM.y, 0);
		}
		if (Input::IsKeyDown(KEY_INPUT_DOWN) || mouseWheelBackRot)
		{
			ScrollOffset_.y -= 1;
			ScrollOffset_.y = std::clamp<int>(ScrollOffset_.y, -toolAreaConf_.MAP_CHIP_NUM.y, 0); // 二行も必要
		}
	}
}

void MapChip::DrawMapPallet()
{
	for (int y = 0; y < toolAreaConf_.MAP_CHIP_NUM.y; y++)
	{
		for (int x = 0; x < toolAreaConf_.MAP_CHIP_NUM.x; x++)
		{
			if (IsInChipArea(mousePosition_))
			{
				DrawBox(toolAreaConf_.TOP_LEFT.x + selected_.x * toolAreaConf_.IMAGE_PX_SIZE, selected_.y * toolAreaConf_.IMAGE_PX_SIZE,
						toolAreaConf_.TOP_LEFT.x + selected_.x * toolAreaConf_.IMAGE_PX_SIZE + toolAreaConf_.IMAGE_PX_SIZE, selected_.y * toolAreaConf_.IMAGE_PX_SIZE + toolAreaConf_.IMAGE_PX_SIZE,
						0xff00ff, true);

				DrawMapChip(x, y);
			}
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
				DrawMapChip(x, y);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

		}
	}

	// マップパレット領域の外枠の描画
	MyDxLib::DrawBox(toolAreaConf_.TOP_LEFT,
					 toolAreaConf_.BOTTOM_RIGHT,
					 toolAreaConf_.FLAME_COLOR,
					 false, 3);
}
