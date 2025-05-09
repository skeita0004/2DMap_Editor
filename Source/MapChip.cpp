#include "MapChip.hpp"
#include "Screen.h"

namespace
{
	// カブってるからどうにかしろ
	static const int IMAGE_SIZE = { 32 };
	const int MAP_WIDTH = { 16 };
	const int MAP_HEIGHT = { 12 };
	const int MAP_CHIP_NUM_WIDTH = { 8 };
	const int MAP_CHIP_NUM_HEIGHT = { 24 };
	const int MAP_WINDOW_WIDTH = { IMAGE_SIZE * MAP_CHIP_NUM_WIDTH };
	const int MAP_WINDOW_HEIGHT = { IMAGE_SIZE * MAP_CHIP_NUM_HEIGHT };
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

}

void MapChip::Draw()
{
	int TOP_LEFT_X = Screen::WIDTH - MAP_WINDOW_WIDTH;
	int TOP_LEFT_Y = 0;
	int RIGHT_BOTTOM_X = Screen::WIDTH;
	int RIGHT_BOTTOM_Y = MAP_WINDOW_HEIGHT;
	
	for (int y = 0; y < MAP_CHIP_NUM_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_CHIP_NUM_WIDTH; x++)
		{
			// 全部出てこない
			DrawGraph(TOP_LEFT_X + x * IMAGE_SIZE + 1, TOP_LEFT_Y + y * IMAGE_SIZE + 1,
						hImage_[x + y * MAP_CHIP_NUM_WIDTH], TRUE);
		}
	}
	DrawBox(TOP_LEFT_X, TOP_LEFT_Y, RIGHT_BOTTOM_X, RIGHT_BOTTOM_Y, 0xff0000, FALSE, 3);
}
