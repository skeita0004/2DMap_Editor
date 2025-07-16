#include "MapEditorConfig.hpp"
#include <Windows.h>
#include "Screen.h"

const MapEditorConfig& GetMapEditorConfig()
{
	static MapEditorConfig conf{ LoadMapEditorConfig( "./profile.ini" ) };
	return conf;
}

const MapEditorConfig& LoadMapEditorConfig( const std::string _iniPath )
{
	MapEditorConfig conf{};
	conf.IMAGE_PX_SIZE = GetPrivateProfileIntA( "MapEdit", "IMAGE_PX_SIZE", 255, _iniPath.c_str() );

	conf.IMAGE_TILE_NUM = { (int)GetPrivateProfileIntA( "MapEdit", "TILE_NUM_WIDTH", 255, _iniPath.c_str() ),
							(int)GetPrivateProfileIntA( "MapEdit", "TILE_NUM_HEIGHT", 255, _iniPath.c_str() ) };
	
	conf.LEFT_POSITION_MARGIN = GetPrivateProfileIntA( "MapEdit", "LEFT_POSITION_MARGIN", 255, _iniPath.c_str() );

	conf.AREA_PX_SIZE = {conf.IMAGE_TILE_NUM.x * conf.IMAGE_PX_SIZE, conf.IMAGE_TILE_NUM.y * conf.IMAGE_PX_SIZE};
	conf.TOP_POSITION_MARGIN = (Screen::HEIGHT - conf.IMAGE_TILE_NUM.y * conf.IMAGE_PX_SIZE) / 2;

	return conf;
}
