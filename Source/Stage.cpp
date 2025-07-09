#include "Stage.h"
#include "Input.h"
#include <DxLib.h>


Stage::Stage() :
	GameObject()
{
	mapEdit_ = new MapEdit();
	mapChip_ = new MapChip();
}

Stage::~Stage()
{
	delete mapChip_;
	mapChip_ = nullptr;
	delete mapEdit_;
	mapEdit_ = nullptr;
}

void Stage::Update()
{
}

void Stage::Draw()
{
#pragma region Debug
	//if (Input::IsMouseDown(MOUSE_INPUT_RIGHT))
	//{
	//	printfDx("inRIGHT:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	//}
	//if (Input::IsMouseDown(MOUSE_INPUT_MIDDLE))
	//{
	//	printfDx("inMIDDLE:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	//}
	//if (Input::IsMouseDown(MOUSE_INPUT_LEFT))
	//{
	//	printfDx("inLEFT:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	//}

	//if (Input::IsMouseUp(MOUSE_INPUT_RIGHT))
	//{
	//	printfDx("outRIGHT:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	//}
	//if (Input::IsMouseUp(MOUSE_INPUT_MIDDLE))
	//{
	//	printfDx("outMIDDLE:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	//}
	//if (Input::IsMouseUp(MOUSE_INPUT_LEFT))
	//{
	//	printfDx("outLEFT:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	//}

	//if (Input::IsMouseHold(MOUSE_INPUT_RIGHT))
	//{
	//	printfDx("holdRIGHT:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	//}
	//if (Input::IsMouseHold(MOUSE_INPUT_MIDDLE))
	//{
	//	printfDx("holdMIDDLE:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	//}
	//if (Input::IsMouseHold(MOUSE_INPUT_LEFT))
	//{
	//	printfDx("holdLEFT:(%d, %d)\n", mousePointer_->GetPosition().x, mousePointer_->GetPosition().y);
	//}
#pragma endregion

}
//-----------//
//   E O F   //
//-----------//