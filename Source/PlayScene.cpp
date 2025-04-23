#include "PlayScene.h"
#include <DxLib.h>
#include "Stage.h"

PlayScene::PlayScene()
{
	//Stage* stage;
	new Stage();
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{

}
