#include "Stage.h"
#include <vector>
#include <cassert>

namespace
{
	static const int IMAGE_SIZE = { 32 };
	const int MAP_X = { 10 };
	const int MAP_Y = { 10 };
	const int MYMAP[MAP_Y][MAP_X]
	{
		{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 10},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 20, 123, 191, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
}

Stage::Stage() :
	hImage_(-1)
{
	hImage_ = LoadGraph("Assets/img/bg.png"); // 記号は使えない、許せない！！
	assert(hImage_ > 0);

	
}

Stage::~Stage()
{
	hImage_ = -1;
}

void Stage::Init()
{

}

void Stage::Update()
{
}

void Stage::Draw()
{
	int imgSizeX = 0;
	int imgSizeY = 0;
	int imgNumX = 0;
	int imgNumY = 0;

	GetGraphSize(hImage_, &imgSizeX, &imgSizeY);
	//while (true)
	//{
	//	static int i = 0;
	//	if (imgSizeX % i == 0)
	//	{
	//		imgNumX = i;
	//		break;
	//	}
	//}

	for (int y = 0; y < MAP_Y; y++)
	{
		for (int x = 0; x < MAP_X; x++)
		{
			int request = MYMAP[y][x];
			int sx = request % 16;
			int sy = request / 12;
			DrawRectGraph(x * IMAGE_SIZE, y * IMAGE_SIZE
						, sx * IMAGE_SIZE, sy * IMAGE_SIZE
						, IMAGE_SIZE, IMAGE_SIZE
						, hImage_, TRUE);
		}
	}
}

void Stage::Release()
{
}
