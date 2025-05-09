#include "Stage.h"

Stage::Stage() :
	GameObject()
{
	mapChip_ = new MapChip();
	mapEdit_ = new MapEdit();
}

Stage::~Stage()
{
	delete mapChip_;
	delete mapEdit_;
}

void Stage::Update()
{
}

void Stage::Draw()
{


}

//-----------//
//   E O F   //
//-----------//