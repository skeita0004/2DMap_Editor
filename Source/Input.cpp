#include "Input.h"
#include "DxLib.h"

namespace Input 
{
	//キーボード取得関連
	const int KEY_MAX = 255;
	char keyBuff[KEY_MAX];		//キーバッファ (現フレーム)
	char keyBuffOld[KEY_MAX];	//1つ前のキーバッファ(前フレーム) 
	char key_down[KEY_MAX];		//押された瞬間 
	char key_up[KEY_MAX];		//離された瞬間 
	char Key_Keep[KEY_MAX];		//押しっぱなし

	int moucePrev = 0;
	int mouceCurr = 0;
	bool mouceDown;
	bool mouceUp;
	bool mouceHold;
}

void Input::KeyStateUpdate()
{
	memcpy_s(keyBuffOld, sizeof(char) * KEY_MAX, keyBuff, sizeof(char) * KEY_MAX); // 配列のコピー

	GetHitKeyStateAll(keyBuff);//全てのキーの状態を取得 

	for (int i = 0; i < KEY_MAX; i++)
	{
		if (keyBuff[i] && keyBuffOld[i]) // 押しっぱなし判定（AND演算）
		{
			Key_Keep[i]++;
		}

		int key_xor = keyBuff[i] ^ keyBuffOld[i];	//前フレームと現フレームのxor
		if (key_xor)
		{
			Key_Keep[i] = 0;
		}
		key_down[i] = key_xor & keyBuff[i];		//押された瞬間 = (現フレームとkey_xorのAND) 
		key_up[i] = key_xor & keyBuffOld[i];	//離された瞬間 = (前フレームとkey_xorのAND) 
	}

	moucePrev = mouceCurr;
	mouceCurr = GetMouseInput();
}

bool Input::IsKeyUP(int keyCode)
{
	return(key_up[keyCode]); // ここでは計算済みの配列の中身を見ている
}

bool Input::IsKeyDown(int keyCode)
{
	return(key_down[keyCode]);
}

bool Input::IsKeyHold(int keyCode)
{
	return(Key_Keep[keyCode]);
}

// マウス系関数
bool Input::IsMouseDown(int _mouceKeyCode)
{
	return mouceDown = ((mouceCurr and _mouceKeyCode) xor (moucePrev and _mouceKeyCode) and (mouceCurr and _mouceKeyCode));
}

bool Input::IsMouseUp(int _mouceKeyCode)
{
	return mouceUp = (mouceCurr xor moucePrev) and moucePrev;
}

bool Input::IsMouseHold(int _mouceKeyCode)
{
	return mouceHold;
}

// 改良できそうなところ…
//		配列をvectorに
//			memcopyはもう古い