#include "MapEdit.hpp"
#include <cassert>

MapEdit::MapEdit() :
	GameObject(),
	myMap_(MAP_CHIP_NUM_WIDTH * MAP_CHIP_NUM_HEIGHT, -1)
{
	//	//hImage_ = new int[MAP_X * MAP_Y]; // ìÆìIÉÅÉÇÉäämï€
////hImage_.resize(MAP_X * MAP_Y);
//	hImage_ = std::vector<int>(MAP_WIDTH * MAP_HEIGHT, -1);
//
//	LoadDivGraph("Assets/img/bg.png",
//		MAP_WIDTH * MAP_HEIGHT,
//		MAP_WIDTH, MAP_HEIGHT,
//		IMAGE_SIZE, IMAGE_SIZE,
//		hImage_.data());

}

MapEdit::~MapEdit()
{
	//for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
	//{
	//	if (hImage_[i] != -1)
	//	{
	//		DeleteGraph(hImage_[i]);
	//		hImage_[i] = -1;
	//	}
	//}
	////delete[] hImage_; // îzóÒâï˙
	
}

void MapEdit::Update()
{

}

void MapEdit::Draw()
{
	//for (int y = 0; y < MAP_Y; y++)
	//{
	//	for (int x = 0; x < MAP_X; x++)
	//	{
	//		int request = MYMAP[y][x];
	//		if (hImage_[request] != -1)
	//		{
	//			DrawGraph(x * IMAGE_SIZE, y * IMAGE_SIZE, hImage_[request], TRUE);
	//		}
	//	}
	//}
	DrawBox(MAP_EDITOR_LEFT_MARGIN, MAP_EDITOR_TOP_MARGIN,
			MAP_EDITOR_LEFT_MARGIN + MAP_EDITOR_WIDTH,
			MAP_EDITOR_TOP_MARGIN + MAP_EDITOR_HEIGHT,
			0xffffff, FALSE, 3);
	for (int i = 0; i < MAP_EDITOR_WIDTH / MAP_CHIP_IMAGE_SIZE; i++)
	{
		DrawLine(MAP_EDITOR_LEFT_MARGIN + i * MAP_CHIP_IMAGE_SIZE, MAP_EDITOR_TOP_MARGIN,
				 MAP_EDITOR_LEFT_MARGIN + i * MAP_CHIP_IMAGE_SIZE, MAP_EDITOR_TOP_MARGIN + MAP_EDITOR_HEIGHT,
				 0xffeedd, 1);
	}

	for (int i = 0; i < MAP_EDITOR_HEIGHT / MAP_CHIP_IMAGE_SIZE; i++)
	{
		DrawLine(MAP_EDITOR_LEFT_MARGIN, MAP_EDITOR_TOP_MARGIN + i * MAP_CHIP_IMAGE_SIZE,
				MAP_EDITOR_LEFT_MARGIN + MAP_EDITOR_WIDTH, MAP_EDITOR_TOP_MARGIN + i * MAP_CHIP_IMAGE_SIZE,
				 0xffeedd, 1);
	}
}
