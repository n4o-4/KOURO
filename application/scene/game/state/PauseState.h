#pragma once
#include "ISceneState.h"
#include "BaseScene.h"

class PauseState : public Kouro::ISceneState
{
public:

	/// \brief ポーズ状態に入るときの処理
	void OnEnter(Kouro::BaseScene* scene) override;

	/// \brief ポーズ状態の更新処理
	void Update() override;

	/// \brief ポーズ状態から出るときの処理
	void OnExit() override;

private:

	static constexpr GameScene::GameSceneUpdateFlags enterFlags_ = { false ,false };

	Kouro::BaseScene* scene_ = nullptr;

};

