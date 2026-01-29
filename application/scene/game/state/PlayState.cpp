#include "PlayState.h"
#include "BaseScene.h"
#include "GameScene.h"

void PlayState::OnEnter(Kouro::BaseScene* scene)
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

	gameScene->SetUpdateFlags({ true,true,true });
}

void PlayState::Update()
{
	// シーンをゲームシーンに型変換
	GameScene* gameScene = dynamic_cast<GameScene*>(scene_);
	
	// 型変換に失敗したら抜ける
	if (!gameScene) return;
	
	gameScene->GetPlayer().Update();


}

void PlayState::Draw()
{
}

void PlayState::OnExit()
{
}
