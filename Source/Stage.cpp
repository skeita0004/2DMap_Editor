#include "Stage.h"
#include "Input.h"
#include <DxLib.h>

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
	printfDx("マウス：%d\n", Input::IsMouseDown(MOUSE_INPUT_LEFT));
}

//-----------//
//   E O F   //
//-----------//