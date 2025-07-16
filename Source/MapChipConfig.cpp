#include "MapChipConfig.hpp"
#include <Windows.h>
#include "Screen.h"

const MapChipConfig& GetMapChipConfig()
{
	static MapChipConfig conf{LoadMapChipConfig("./profile.ini")};
	return conf;
}

const MapChipConfig& LoadMapChipConfig(const std::string _iniPath)
{
	MapChipConfig conf{};
	conf.IMAGE_PX_SIZE = GetPrivateProfileIntA("MapChip", "IMAGE_PX_SIZE", 255, _iniPath.c_str());

	conf.IMAGE_TILE_NUM = { (int)GetPrivateProfileIntA("MapChip", "IMAGE_TILE_NUM_WIDTH", 255, _iniPath.c_str()),
							(int)GetPrivateProfileIntA("MapChip", "IMAGE_TILE_NUM_HEIGHT", 255, _iniPath.c_str()) };

	conf.MAP_CHIP_NUM = { (int)GetPrivateProfileIntA("MapChip", "MAP_CHIP_NUM_WIDTH", 255, _iniPath.c_str()),
						  (int)GetPrivateProfileIntA("MapChip", "MAP_CHIP_NUM_HEIGHT", 255, _iniPath.c_str()) };

	conf.FLAME_COLOR = GetPrivateProfileIntA("MapChip", "FLAME_COLOR", 255, _iniPath.c_str());

	conf.MAP_WINDOW_SIZE = { conf.IMAGE_PX_SIZE * conf.MAP_CHIP_NUM.x,
							   conf.IMAGE_PX_SIZE * conf.MAP_CHIP_NUM.y };

	conf.TOP_LEFT = { Screen::WIDTH - conf.MAP_WINDOW_SIZE.x, 0 };
	
	conf.BOTTOM_RIGHT = { Screen::WIDTH, conf.MAP_WINDOW_SIZE.y };
	return conf;
}
