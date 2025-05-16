#include "Input.h"
#include "DxLib.h"

namespace Input 
{
	//�L�[�{�[�h�擾�֘A
	const int KEY_MAX = 255;
	char keyBuff[KEY_MAX];		//�L�[�o�b�t�@ (���t���[��)
	char keyBuffOld[KEY_MAX];	//1�O�̃L�[�o�b�t�@(�O�t���[��) 
	char key_down[KEY_MAX];		//�����ꂽ�u�� 
	char key_up[KEY_MAX];		//�����ꂽ�u�� 
	char Key_Keep[KEY_MAX];		//�������ςȂ�

	int moucePrev = 0;
	int mouceCurr = 0;
	bool mouceDown;
	bool mouceUp;
	bool mouceHold;
}

void Input::KeyStateUpdate()
{
	memcpy_s(keyBuffOld, sizeof(char) * KEY_MAX, keyBuff, sizeof(char) * KEY_MAX); // �z��̃R�s�[

	GetHitKeyStateAll(keyBuff);//�S�ẴL�[�̏�Ԃ��擾 

	for (int i = 0; i < KEY_MAX; i++)
	{
		if (keyBuff[i] && keyBuffOld[i]) // �������ςȂ�����iAND���Z�j
		{
			Key_Keep[i]++;
		}

		int key_xor = keyBuff[i] ^ keyBuffOld[i];	//�O�t���[���ƌ��t���[����xor
		if (key_xor)
		{
			Key_Keep[i] = 0;
		}
		key_down[i] = key_xor & keyBuff[i];		//�����ꂽ�u�� = (���t���[����key_xor��AND) 
		key_up[i] = key_xor & keyBuffOld[i];	//�����ꂽ�u�� = (�O�t���[����key_xor��AND) 
	}

	moucePrev = mouceCurr;
	mouceCurr = GetMouseInput();
}

bool Input::IsKeyUP(int keyCode)
{
	return(key_up[keyCode]); // �����ł͌v�Z�ς݂̔z��̒��g�����Ă���
}

bool Input::IsKeyDown(int keyCode)
{
	return(key_down[keyCode]);
}

bool Input::IsKeyHold(int keyCode)
{
	return(Key_Keep[keyCode]);
}

// �}�E�X�n�֐�
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

// ���ǂł������ȂƂ���c
//		�z���vector��
//			memcopy�͂����Â�