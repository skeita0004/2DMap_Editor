#include "Stage.h"
#include "Input.h"
#include <DxLib.h>


Stage::Stage() :
	GameObject()
{
	mapChip_ = new MapChip();
	mapEdit_ = new MapEdit();
	mousePointer_ = new MousePointer();
}

Stage::~Stage()
{
	delete mapChip_;
	delete mapEdit_;
	delete mousePointer_;
}

void Stage::Update()
{
}

void Stage::Draw()
{
	if (Input::IsMouseDown(MOUSE_INPUT_RIGHT))
	{
		printfDx("inRIGHT:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	}
	if (Input::IsMouseDown(MOUSE_INPUT_MIDDLE))
	{
		printfDx("inMIDDLE:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	}
	if (Input::IsMouseDown(MOUSE_INPUT_LEFT))
	{
		printfDx("inLEFT:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	}

	if (Input::IsMouseUp(MOUSE_INPUT_RIGHT))
	{
		printfDx("outRIGHT:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	}
	if (Input::IsMouseUp(MOUSE_INPUT_MIDDLE))
	{
		printfDx("outMIDDLE:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	}
	if (Input::IsMouseUp(MOUSE_INPUT_LEFT))
	{
		printfDx("outLEFT:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	}

	if (Input::IsMouseHold(MOUSE_INPUT_RIGHT))
	{
		printfDx("holdRIGHT:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	}
	if (Input::IsMouseHold(MOUSE_INPUT_MIDDLE))
	{
		printfDx("holdMIDDLE:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	}
	if (Input::IsMouseHold(MOUSE_INPUT_LEFT))
	{
		printfDx("holdLEFT:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	}
}
//-----------//
//   E O F   //
//-----------//