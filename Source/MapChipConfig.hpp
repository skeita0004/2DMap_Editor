#pragma once

#include "../Library/Vector2D.hpp"
#include <string>

struct MapChipConfig
{
	int IMAGE_PX_SIZE;
	Vector2D<int> IMAGE_TILE_NUM;
	Vector2D<int> MAP_CHIP_NUM;
	int FLAME_COLOR;
	Vector2D<int> MAP_WINDOW_SIZE;
	Vector2D<int> TOP_LEFT;
	Vector2D<int> RIGHT_BOTTOM;
};

// �Q�Ƃ��Ԃ��Ă���̂ŁA�v�Z������
const MapChipConfig& GetMapChipConfig();

const MapChipConfig& LoadMapChipConfig(const std::string _iniPath);