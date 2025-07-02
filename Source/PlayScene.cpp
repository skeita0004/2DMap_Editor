#include "PlayScene.h"
#include <DxLib.h>
#include "Stage.h"
#include "Input.h"

PlayScene::PlayScene()
{
	//Stage* stage;
	new Stage();
	//SetMouseDispFlag(FALSE); // カーソル非表示
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	Input::KeyStateUpdate();
	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{

}
