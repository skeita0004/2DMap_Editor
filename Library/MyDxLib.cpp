#include "MyDxLib.hpp"
#include <DxLib.h>

int MyDxLib::DrawExtendGraph(Vector2D<float> _position, Vector2D<int> _imageSize, int _graphHandle, bool _isTrans)
{
    return DxLib::DrawExtendGraphF(_position.x, _position.y,
                                   _position.x + (float)_imageSize.x, _position.y + (float)_imageSize.y,
                                   _graphHandle, (int)_isTrans);
}

int MyDxLib::DrawBox(const Vector2D<int>& _startPos, const Vector2D<int>& _endPos, const uint32_t _color, const bool _fillFlag, uint32_t _lineThickness)
{
    return DxLib::DrawBox(_startPos.x, _startPos.y, _endPos.x, _endPos.y, _color, _fillFlag, _lineThickness);
}

