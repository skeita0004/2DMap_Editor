#pragma once
#include "../Library/GameObject.h"
class Stage : public GameObject
{
public:
	Stage();
	~Stage();

	void Init();
	void Update();
	void Draw();
	void Release();


private:
	int hImage_;

};

