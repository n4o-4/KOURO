#pragma once
#include "ISceneState.h"

class PlayState : public Kouro::ISceneState
{
public:

	/// \brief ポーズ状態に入るときの処理
	void OnEnter(Kouro::BaseScene* scene) override;

	/// \brief ポーズ状態の更新処理
	void Update() override;

	/// \brief ポーズ状態の描画処理
	void Draw() override;

	/// \brief ポーズ状態から出るときの処理
	void OnExit() override;
};

