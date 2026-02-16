#pragma once
#include "ICameraState.h"

class LookAtPlayerState : public ICameraState
{
public: // メンバ関数

	/**
	* \brief  コンストラクタ
	* \param  camera
	* \param  target
	*/
	LookAtPlayerState(Kouro::BaseCamera* camera, const Kouro::WorldTransform* target) : ICameraState(camera, target) {};

	/// \brief 開始処理
	void Enter() override;

	void Update(float deltaTime) override;

	void Exit() override;

};

