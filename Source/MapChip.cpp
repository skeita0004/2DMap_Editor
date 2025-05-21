#include "MapChip.hpp"
#include "Screen.h"
#include "MousePointer.hpp"
#include "Input.h"

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
	GameObject()
{
	hImage_ = std::vector<int>(MAP_WIDTH * MAP_HEIGHT, -1);

	LoadDivGraph("Assets/img/bg.png",
				  MAP_WIDTH * MAP_HEIGHT,
				  MAP_WIDTH, MAP_HEIGHT,
				  IMAGE_SIZE, IMAGE_SIZE,
				  hImage_.data());
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
	int x = FindGameObject<MousePointer>()->GetPosition().x;
	int y = FindGameObject<MousePointer>()->GetPosition().y;
	if (x >= TOP_LEFT_X && y >= TOP_LEFT_Y && x <= RIGHT_BOTTOM_X && y <= RIGHT_BOTTOM_Y)
	{
		isAlpha = true;
	}
	else
	{
		isAlpha = false;
	}

	if (true)
	{
		// int n = 
		if (Input::IsMouseDown(MOUSE_INPUT_LEFT))
		{
			// DrawGraph(x, y, hImage[n], TRUE);
		}
	}
}

void MapChip::Draw()
{
	
	for (int y = 0; y < MAP_CHIP_NUM_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_CHIP_NUM_WIDTH; x++)
		{
			
			if (isAlpha == false)
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
			else
			{
				DrawGraph(TOP_LEFT_X + x * IMAGE_SIZE + 1, TOP_LEFT_Y + y * IMAGE_SIZE + 1,
					hImage_[x + y * MAP_CHIP_NUM_WIDTH], TRUE);
				DrawLine(TOP_LEFT_X + IMAGE_SIZE * x, TOP_LEFT_Y,
					TOP_LEFT_X + IMAGE_SIZE * x, TOP_LEFT_Y + MAP_WINDOW_HEIGHT,
					0xffffff);
				DrawLine(TOP_LEFT_X, TOP_LEFT_Y + IMAGE_SIZE * y,
					TOP_LEFT_X + MAP_WINDOW_WIDTH, TOP_LEFT_Y + IMAGE_SIZE * y,
					0xffffff);
			}
		}
	}
	DrawBox(TOP_LEFT_X, TOP_LEFT_Y, RIGHT_BOTTOM_X, RIGHT_BOTTOM_Y, FLAME_COLOR, FALSE, 3);
}
