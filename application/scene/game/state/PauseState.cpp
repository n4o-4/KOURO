#include "PauseState.h"
#include "GameScene.h"

void PauseState::OnEnter(Kouro::BaseScene* scene)
{
	// 引数をメンバ変数に記録
	scene_ = scene;

	// シーンの登録が出来ているか確認
	if (!scene_)
	{
		// nullだった場合エラー
		assert(0);
	}

	// シーンをゲームシーンに型変換
	GameScene* gameScene = dynamic_cast<GameScene*>(scene_);

	// ゲームシーンに型変換出来ているか確認
	if (!gameScene)
	{
		return;
	}

	gameScene->SetUpdateFlags(enterFlags_);
}

void PauseState::Update()
{
}

void PauseState::OnExit()
{
}