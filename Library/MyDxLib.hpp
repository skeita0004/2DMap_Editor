#pragma once

#include "Vector2D.hpp"
#include <cstdint>

namespace MyDxLib
{
	/// <summary>
	/// MyDxLibのDrawExtendGraph関数
	/// 引数をVector2で渡せるので、記述量を減らせる
	/// </summary>
	/// <param name="_position">対象の座標</param>
	/// <param name="_imageSize">対象の大きさ</param>
	/// <param name="_graphHandle">対象の画像ハンドル</param>
	/// <param name="_isTrans">透過させるか</param>
	/// <returns>エラーコード</returns>
	int DrawExtendGraph(Vector2D<float> _position, Vector2D<int> _imageSize, int _graphHandle, bool _isTrans);
	
	/// <summary>
	/// MyDxLibのDrawBox関数
	/// </summary>
	/// <param name="_startPos">矩形の始点</param>
	/// <param name="_endPos">矩形の終点</param>
	/// <param name="_color">矩形の色</param>
	/// <param name="_fillFlag">塗りつぶしの有無</param>
	/// <returns>正常:0, 以上:-1</returns>
	int DrawBox(const Vector2D<int>& _startPos, const Vector2D<int>& _endPos, const uint32_t _color, const bool _fillFlag, uint32_t _lineThickness = 1);
}