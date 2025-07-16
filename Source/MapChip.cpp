#define NOMINMAX

#include "MapChip.hpp"
#include "Screen.h"
#include "Input.h"
#include <map>
#include "../Library/MyDxLib.hpp"
#include <algorithm>

namespace
{
	bool isAlpha = false;
}

MapChip::MapChip() :
	GameObject(),
	hImage_(std::vector<int>(0, -1)),
	handleToIndex_(),
	mousePosition_({0, 0}),
	isHold_(false),
	heldIndex_(-1),
	selected_({0, 0}),
	mcg_(GetMapChipConfig()),
	ScrollOffset_({0, 0})
{
	char buff[255];
	GetPrivateProfileStringA("MapChip", "Title", "game", 
							 buff, sizeof(buff), "./profile.ini");
	//int a = GetPrivateProfileIntA("MapChip", "ImageSize", 32, "./profile.ini");
	hImage_ = std::vector<int>(mcg_.IMAGE_TILE_NUM.x * mcg_.IMAGE_TILE_NUM.y, -1);
	handleToIndex_.clear();

	LoadDivGraph("Assets/img/bg.png",
		mcg_.IMAGE_TILE_NUM.x * mcg_.IMAGE_TILE_NUM.y,
		mcg_.IMAGE_TILE_NUM.x, mcg_.IMAGE_TILE_NUM.y,
		mcg_.IMAGE_PX_SIZE, mcg_.IMAGE_PX_SIZE,
		hImage_.data());

	for (int i = 0; i < hImage_.size(); i++)
	{
		handleToIndex_.insert(std::make_pair(hImage_[i], i));
	}

	std::map<int, int> handleDatabase();
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
}

void MapChip::Update()
{
	GetMousePoint(&mousePosition_.x, &mousePosition_.y);

	selected_ = ScreenToMapIndex(mousePosition_);

#pragma region IsInMapChipArea_

	if (IsInChipArea(mousePosition_))
	{
		isAlpha = true;
		if (Input::IsMouseDown(MOUSE_INPUT_LEFT))
		{
			isHold_ = true;
			ToLocalPos(selected_);
			heldIndex_ = selected_.y * mcg_.MAP_CHIP_NUM.x + selected_.x;
		}
	}
	else
	{
		isAlpha = false;
	}

	if (isAlpha)
	{
		if (Input::IsMouseDown(MOUSE_INPUT_RIGHT))
		{
			heldIndex_ = -1;
		}
	}

	if (Input::IsKeyDown(KEY_INPUT_LEFT))
	{
		ScrollOffset_.x = std::max(0, ScrollOffset_.x - 1);
	}
	if (Input::IsKeyDown(KEY_INPUT_RIGHT))
	{
		ScrollOffset_.x = std::min(mcg_.MAP_CHIP_NUM.x, ScrollOffset_.x + 1); // àÍçsÇ≈çœÇﬁ
	}

	if (Input::IsKeyDown( KEY_INPUT_UP ))
	{
		ScrollOffset_.y += 1;
		ScrollOffset_.y = std::clamp<int>( ScrollOffset_.y, -mcg_.MAP_CHIP_NUM.y, 0 );
	}
	if (Input::IsKeyDown( KEY_INPUT_DOWN ))
	{
		ScrollOffset_.y -= 1;
		ScrollOffset_.y = std::clamp<int>(ScrollOffset_.y, -mcg_.MAP_CHIP_NUM.y, 0); // ìÒçsÇ‡Ç¢ÇÈ
	}
#pragma endregion
}

void MapChip::Draw()
{
	for (int y = 0; y < mcg_.IMAGE_TILE_NUM.y; y++)
	{
		for (int x = 0; x < mcg_.IMAGE_TILE_NUM.x; x++)
		{
			if (isAlpha)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

				DrawBox(mcg_.TOP_LEFT.x + selected_.x * mcg_.IMAGE_PX_SIZE, selected_.y * mcg_.IMAGE_PX_SIZE,
					mcg_.TOP_LEFT.x + selected_.x * mcg_.IMAGE_PX_SIZE + mcg_.IMAGE_PX_SIZE, selected_.y * mcg_.IMAGE_PX_SIZE + mcg_.IMAGE_PX_SIZE,
					0xff00ff, false, 2);
				DrawBox(mcg_.TOP_LEFT.x + selected_.x * mcg_.IMAGE_PX_SIZE, selected_.y * mcg_.IMAGE_PX_SIZE,
					mcg_.TOP_LEFT.x + selected_.x * mcg_.IMAGE_PX_SIZE + mcg_.IMAGE_PX_SIZE, selected_.y * mcg_.IMAGE_PX_SIZE + mcg_.IMAGE_PX_SIZE,
					0xff00ff, true);

				DrawMapChip( x, y );

				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			else
			{
				DrawMapChip( x, y );
			}

		}
	}

	for (int ly = 0; ly < mcg_.IMAGE_TILE_NUM.y * 2; ly++)
	{
		DrawLine( mcg_.TOP_LEFT.x, mcg_.TOP_LEFT.y + mcg_.IMAGE_PX_SIZE * ly,
				  mcg_.TOP_LEFT.x + mcg_.MAP_WINDOW_SIZE.x, mcg_.TOP_LEFT.y + mcg_.IMAGE_PX_SIZE * ly,
				  0xffffff );
	}

	MyDxLib::DrawBox(mcg_.TOP_LEFT, mcg_.BOTTOM_RIGHT, mcg_.FLAME_COLOR, false, 3);
	MyDxLib::DrawBox( { mcg_.TOP_LEFT.x, mcg_.MAP_CHIP_NUM.y * mcg_.IMAGE_PX_SIZE }, { mcg_.BOTTOM_RIGHT.x, Screen::HEIGHT }, 0x0, true );

	if (isHold_ && heldIndex_ != -1)
	{
		MyDxLib::DrawExtendGraph(mousePosition_, { mcg_.IMAGE_PX_SIZE, mcg_.IMAGE_PX_SIZE },
			hImage_[heldIndex_], true);
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
	return (_mouse.x >= mcg_.TOP_LEFT.x && _mouse.y >= mcg_.TOP_LEFT.y && _mouse.x <= mcg_.BOTTOM_RIGHT.x && _mouse.y <= mcg_.BOTTOM_RIGHT.y);
}

Vector2D<int> MapChip::ScreenToMapIndex(const Vector2D<int>& _mouse)
{
	return { (_mouse.x - (Screen::WIDTH - mcg_.MAP_WINDOW_SIZE.x)) / mcg_.IMAGE_PX_SIZE,
			  _mouse.y / mcg_.IMAGE_PX_SIZE };
}

void MapChip::DrawMapChip(const int& _x, const int& _y)
{
	int x = _x;
	int y = _y;
	DrawGraph(mcg_.TOP_LEFT.x + x * mcg_.IMAGE_PX_SIZE,
			 (mcg_.TOP_LEFT.y + y ) * mcg_.IMAGE_PX_SIZE,
			 hImage_[std::min( x + ScrollOffset_.x, mcg_.IMAGE_TILE_NUM.x - 1 ) + std::min(y + abs(ScrollOffset_.y), mcg_.IMAGE_TILE_NUM.y - 1) * mcg_.IMAGE_TILE_NUM.x], TRUE );

	int verticalLineStartX = mcg_.TOP_LEFT.x + mcg_.IMAGE_PX_SIZE * x;

	DrawLine( verticalLineStartX, mcg_.TOP_LEFT.y,
			  verticalLineStartX, mcg_.TOP_LEFT.y + mcg_.MAP_WINDOW_SIZE.y,
			  0xffffff );
}

void MapChip::ToLocalPos( Vector2D<float>& _pos )
{
	_pos.x = _pos.x - ScrollOffset_.x;
	_pos.y = _pos.y + abs(ScrollOffset_.y);
}
