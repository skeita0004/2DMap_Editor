#pragma once
#include "../Library/GameObject.h"
#include <vector>
#include "MapChip.hpp"

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
	//int *hImage_; //�w�i�摜�̃n���h���z��ւ̃|�C���^
	std::vector<int> hImage_;
	MapChip* mapChip_;
};

