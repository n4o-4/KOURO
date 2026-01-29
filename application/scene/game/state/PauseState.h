#pragma once
#include "ISceneState.h"

#include <string>
#include <functional>

class PauseState : public Kouro::ISceneState
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

	/**
	* \brief  ポーズ状態から出るときのコールバック関数を設定する
	* \param  callback : コールバック関数
	*/
	void SetOnExitCallback(std::function<void(const std::string&)> callback) { onExitCallback_ = callback; }

private:

	std::function<void(const std::string&)> onExitCallback_;

};