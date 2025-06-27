#include "MapChip.hpp"
#include "Screen.h"
#include "Input.h"
#include <map>

namespace
{
	// ƒJƒu‚Á‚Ä‚é‚©‚ç‚Ç‚¤‚É‚©‚µ‚ë
	static const int IMAGE_SIZE = { 32 };
	const int MAP_WIDTH = { 16 };
	const int MAP_HEIGHT = { 12 };
	const int MAP_CHIP_NUM_WIDTH = { 8 };
	const int MAP_CHIP_NUM_HEIGHT = { 24 };
	const int MAP_WINDOW_WIDTH = { IMAGE_SIZE * MAP_CHIP_NUM_WIDTH };
	const int MAP_WINDOW_HEIGHT = { IMAGE_SIZE * MAP_CHIP_NUM_HEIGHT };
	const int FLAME_COLOR = GetColor(255, 0, 0);
	bool      isAlpha = false;

	const int TOP_LEFT_X = Screen::WIDTH - MAP_WINDOW_WIDTH;
	const int TOP_LEFT_Y = 0;
	const int RIGHT_BOTTOM_X = Screen::WIDTH;
	const int RIGHT_BOTTOM_Y = MAP_WINDOW_HEIGHT;
}

MapChip::MapChip() :
	GameObject(),
	mousePosition_({0, 0}),
	selected_({0, 0}),
	holdedIndex_(-1),
	isHold_(false),
	hImage_(std::vector<int>(0, -1))
{
	hImage_ = std::vector<int>(MAP_WIDTH * MAP_HEIGHT, -1);

	LoadDivGraph("Assets/img/bg.png",
				  MAP_WIDTH * MAP_HEIGHT,
				  MAP_WIDTH, MAP_HEIGHT,
				  IMAGE_SIZE, IMAGE_SIZE,
				  hImage_.data());

	std::map<int, int> handleDatabase();

}

MapChip::~MapChip()
{
	for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
	{
		if (hImage_[i] != -1)
		{
			DeleteGraph(hImage_[i]);
			hImage_[i] = -1;
		}
	}
}

void MapChip::Update()
{
	GetMousePoint(&mousePosition_.x, &mousePosition_.y);

	selected_.x = (mousePosition_.x - (Screen::WIDTH - MAP_WINDOW_WIDTH)) / IMAGE_SIZE;
	selected_.y = mousePosition_.y / IMAGE_SIZE;

#pragma region IsInMapChipArea_
	int x = mousePosition_.x;
	int y = mousePosition_.y;
	if (x >= TOP_LEFT_X && y >= TOP_LEFT_Y && x <= RIGHT_BOTTOM_X && y <= RIGHT_BOTTOM_Y)
	{
		isAlpha = true;
		if (Input::IsMouseDown(MOUSE_INPUT_LEFT))
		{
			isHold_ = true;
			holdedIndex_ = selected_.y * MAP_CHIP_NUM_WIDTH + selected_.x;
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

	//VECTOR2INT mousePosition;
	//while (GetMousePoint(&mousePosition.x, &mousePosition.y) == -1)
	//{
	//}


#pragma endregion
}

void MapChip::Draw()
{
	// int select mousep.x - scrwid - mchipwidth / imagesize
	// int select.y mousep.y / imagesize


	for (int y = 0; y < MAP_CHIP_NUM_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_CHIP_NUM_WIDTH; x++)
		{
			
			if (isAlpha)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 500);

				DrawBox(TOP_LEFT_X + selected_.x * IMAGE_SIZE, selected_.y * IMAGE_SIZE,
					TOP_LEFT_X + selected_.x * IMAGE_SIZE + IMAGE_SIZE, selected_.y * IMAGE_SIZE + IMAGE_SIZE,
					0xff00ff, false, 2);
				DrawBox(TOP_LEFT_X + selected_.x * IMAGE_SIZE, selected_.y * IMAGE_SIZE,
					TOP_LEFT_X + selected_.x * IMAGE_SIZE + IMAGE_SIZE, selected_.y * IMAGE_SIZE + IMAGE_SIZE,
					0xff00ff, true);
				DrawGraph(TOP_LEFT_X + x * IMAGE_SIZE + 1, TOP_LEFT_Y + y * IMAGE_SIZE + 1,
					hImage_[x + y * MAP_CHIP_NUM_WIDTH], TRUE);

				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				

				DrawLine(TOP_LEFT_X + IMAGE_SIZE * x, TOP_LEFT_Y,
					TOP_LEFT_X + IMAGE_SIZE * x, TOP_LEFT_Y + MAP_WINDOW_HEIGHT,
					0xffffff);
				DrawLine(TOP_LEFT_X, TOP_LEFT_Y + IMAGE_SIZE * y,
					TOP_LEFT_X + MAP_WINDOW_WIDTH, TOP_LEFT_Y + IMAGE_SIZE * y,
					0xffffff);

			}
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);

				DrawGraph(TOP_LEFT_X + x * IMAGE_SIZE + 1, TOP_LEFT_Y + y * IMAGE_SIZE + 1,
							hImage_[x + y * MAP_CHIP_NUM_WIDTH], TRUE);
				DrawLine(TOP_LEFT_X + IMAGE_SIZE * x, TOP_LEFT_Y,
					TOP_LEFT_X + IMAGE_SIZE * x, TOP_LEFT_Y + MAP_WINDOW_HEIGHT,
					0xffffff);
				DrawLine(TOP_LEFT_X, TOP_LEFT_Y + IMAGE_SIZE * y,
					TOP_LEFT_X + MAP_WINDOW_WIDTH, TOP_LEFT_Y + IMAGE_SIZE * y,
					0xffffff);

				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
		}
	}
	DrawBox(TOP_LEFT_X, TOP_LEFT_Y, RIGHT_BOTTOM_X, RIGHT_BOTTOM_Y, FLAME_COLOR, FALSE, 3);

	if (isHold_)
	{
		DrawExtendGraph(mousePosition_.x, mousePosition_.y,
			mousePosition_.x + IMAGE_SIZE, mousePosition_.y + IMAGE_SIZE,
			hImage_[holdedIndex_], true);
	}
}

int MapChip::GetHoldImage()
{
	return hImage_[holdedIndex_];
}
