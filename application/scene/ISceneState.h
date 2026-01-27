#pragma once
#include "BaseScene.h"

class ISceneState
{
public:
	/// \brief 仮想デストラクタ
	virtual ~ISceneState() = default;

	/**
	* \brief  状態に入るときの処理
	* \param  scene : 処理対象のシーン
	*/
	virtual void OnEnter(Kouro::BaseScene* scene) = 0;

	/// \brief 状態の更新処理
	virtual void Update() = 0;

	/// \brief 状態から出るときの処理
	virtual void OnExit() = 0;

protected:

	Kouro::BaseScene* scene_ = nullptr;
};