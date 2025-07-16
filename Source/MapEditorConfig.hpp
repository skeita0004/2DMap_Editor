#pragma once
#pragma once

#include "../Library/Vector2D.hpp"
#include <string>

struct MapEditorConfig
{
	int IMAGE_PX_SIZE;
	Vector2D<int> IMAGE_TILE_NUM;
	int LEFT_POSITION_MARGIN;
	Vector2D<int> AREA_PX_SIZE;
	int TOP_POSITION_MARGIN;
};

// �Q�Ƃ��Ԃ��Ă���̂ŁA�v�Z������
const MapEditorConfig& GetMapEditorConfig();

const MapEditorConfig& LoadMapEditorConfig(const std::string _iniPath);