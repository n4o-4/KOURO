#include "PauseState.h"

#include "SceneCommand.h"
#include "BaseScene.h"
#include "GameScene.h"

Kouro::ISceneState::~ISceneState() = default;

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

	gameScene->SetUpdateFlags({ false,false,false });
}

void PauseState::Update()
{
	Kouro::Input* input = Kouro::Input::GetInstance();

	// シーンをゲームシーンに型変換
	GameScene* gameScene = dynamic_cast<GameScene*>(scene_);

	if (input->TriggerKey(DIK_ESCAPE))
	{
		onExitCallback_(SceneCommand::Resume);
	}
	else if (input->TriggerKey(DIK_T))
	{
		onExitCallback_(SceneCommand::Title);
	}
}

void PauseState::Draw()
{

}

void PauseState::OnExit()
{
	// シーンをゲームシーンに型変換
	GameScene* gameScene = dynamic_cast<GameScene*>(this->scene_);

	if (gameScene)
	{
		gameScene->SetUpdateFlags({ true,true,true });
	}
}