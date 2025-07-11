#define NOMINMAX

#include "MapChip.hpp"
#include "Screen.h"
#include "Input.h"
#include <map>
#include "../Library/MyDxLib.hpp"
#include <algorithm>

namespace
{
	//const int IMAGE_SIZE = { 32 };
	//const int MAP_WIDTH = { 16 };
	//const int MAP_HEIGHT = { 12 };
	//const int MAP_CHIP_NUM_WIDTH = { 8 };
	//const int MAP_CHIP_NUM_HEIGHT = { 24 };
	//const int MAP_WINDOW_WIDTH = { IMAGE_SIZE * MAP_CHIP_NUM_WIDTH };
	//const int MAP_WINDOW_HEIGHT = { IMAGE_SIZE * MAP_CHIP_NUM_HEIGHT };
	//const int FLAME_COLOR = GetColor(255, 0, 0);
	bool      isAlpha = false;

	//const int TOP_LEFT_X = Screen::WIDTH - MAP_WINDOW_WIDTH;
	//const int TOP_LEFT_Y = 0;
	//const int RIGHT_BOTTOM_X = Screen::WIDTH;
	//const int RIGHT_BOTTOM_Y = MAP_WINDOW_HEIGHT;
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
	ScrollOffset_(0)
{
	char buff[255];
	GetPrivateProfileStringA("MapChip", "Title", "game", 
							 buff, sizeof(buff), "./profile.ini");
	int a = GetPrivateProfileIntA("MapChip", "ImageSize", 32, "./profile.ini");
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
			
		}
	}

	if (Input::IsKeyDown(KEY_INPUT_LEFT))
	{
		ScrollOffset_ = std::min(mcg_.MAP_CHIP_NUM.x, ScrollOffset_ + 1);
	}
	if (Input::IsKeyDown(KEY_INPUT_RIGHT))
	{
		ScrollOffset_ = std::max(0, ScrollOffset_ - 1);
	}

#pragma endregion
}

void MapChip::Draw()
{
	printfDx("%d\n", GetMouseWheelRotVol());
	for (int y = 0; y < mcg_.IMAGE_TILE_NUM.y; y++)
	{
		for (int x = 0; x < mcg_.IMAGE_TILE_NUM.x; x++)
		{
			if (isAlpha)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 500);

				DrawBox(mcg_.TOP_LEFT.x + selected_.x * mcg_.IMAGE_PX_SIZE, selected_.y * mcg_.IMAGE_PX_SIZE,
					mcg_.TOP_LEFT.x + selected_.x * mcg_.IMAGE_PX_SIZE + mcg_.IMAGE_PX_SIZE, selected_.y * mcg_.IMAGE_PX_SIZE + mcg_.IMAGE_PX_SIZE,
					0xff00ff, false, 2);
				DrawBox(mcg_.TOP_LEFT.x + selected_.x * mcg_.IMAGE_PX_SIZE, selected_.y * mcg_.IMAGE_PX_SIZE,
					mcg_.TOP_LEFT.x + selected_.x * mcg_.IMAGE_PX_SIZE + mcg_.IMAGE_PX_SIZE, selected_.y * mcg_.IMAGE_PX_SIZE + mcg_.IMAGE_PX_SIZE,
					0xff00ff, true);



				DrawGraph(mcg_.TOP_LEFT.x + x * mcg_.IMAGE_PX_SIZE,
						  mcg_.TOP_LEFT.y + y * mcg_.IMAGE_PX_SIZE,
						  hImage_[std::min(x + ScrollOffset_, mcg_.IMAGE_TILE_NUM.x - 1) + y * mcg_.IMAGE_TILE_NUM.x], TRUE);

				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


				DrawLine(mcg_.TOP_LEFT.x + mcg_.IMAGE_PX_SIZE * x, mcg_.TOP_LEFT.y,
					mcg_.TOP_LEFT.x + mcg_.IMAGE_PX_SIZE * x, mcg_.TOP_LEFT.y + mcg_.MAP_WINDOW_SIZE.y,
					0xffffff);
				DrawLine(mcg_.TOP_LEFT.x, mcg_.TOP_LEFT.y + mcg_.IMAGE_PX_SIZE * y,
					mcg_.TOP_LEFT.x + mcg_.MAP_WINDOW_SIZE.x, mcg_.TOP_LEFT.y + mcg_.IMAGE_PX_SIZE * y,
					0xffffff);
			}
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);

				DrawGraph(mcg_.TOP_LEFT.x + x * mcg_.IMAGE_PX_SIZE,
						  mcg_.TOP_LEFT.y + y * mcg_.IMAGE_PX_SIZE,
						  hImage_[x + y * mcg_.IMAGE_TILE_NUM.x], TRUE);

				int verticalLineStartX = mcg_.TOP_LEFT.x + mcg_.IMAGE_PX_SIZE * x;

				DrawLine(verticalLineStartX, mcg_.TOP_LEFT.y,
					verticalLineStartX, mcg_.TOP_LEFT.y + mcg_.MAP_WINDOW_SIZE.y,
					0xffffff);

				DrawLine(mcg_.TOP_LEFT.x, mcg_.TOP_LEFT.y + mcg_.IMAGE_PX_SIZE * y,
					mcg_.TOP_LEFT.x + mcg_.MAP_WINDOW_SIZE.x, mcg_.TOP_LEFT.y + mcg_.IMAGE_PX_SIZE * y,
					0xffffff);

				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}
	MyDxLib::DrawBox(mcg_.TOP_LEFT, mcg_.RIGHT_BOTTOM, mcg_.FLAME_COLOR, false, 3);

	if (isHold_)
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
	return (_mouse.x >= mcg_.TOP_LEFT.x && _mouse.y >= mcg_.TOP_LEFT.y && _mouse.x <= mcg_.RIGHT_BOTTOM.x && _mouse.y <= mcg_.RIGHT_BOTTOM.y);
}

Vector2D<int> MapChip::ScreenToMapIndex(const Vector2D<int>& _mouse)
{
	return { (_mouse.x - (Screen::WIDTH - mcg_.MAP_WINDOW_SIZE.x)) / mcg_.IMAGE_PX_SIZE,
			  _mouse.y / mcg_.IMAGE_PX_SIZE };
}

