#include "MousePointer.hpp"
#include <DxLib.h>

MousePointer::MousePointer() :
	position_()
{
	
}

MousePointer::~MousePointer()
{
}

void MousePointer::Update()
{
	GetMousePoint(&position_.x, &position_.y);
}

void MousePointer::Draw()
{
	DrawCircle(position_.x, position_.y, 5, 0xffffff);
}