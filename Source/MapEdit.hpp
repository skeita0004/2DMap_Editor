#pragma once

#include "../Library/GameObject.h"
#include "Screen.h"
#include <vector>



class MapEdit : public GameObject
{
private:
	//int *hImage_; //�w�i�摜�̃n���h���z��ւ̃|�C���^
	std::vector<int> myMap_;

public:
	MapEdit();
	~MapEdit();

	void Update();
	void Draw();
};