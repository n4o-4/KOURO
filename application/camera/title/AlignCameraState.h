#pragma once
#include "ICameraState.h"

class AlignCameraState : public ICameraState
{
public: // メンバ関数

	/**
	* \brief  コンストラクタ
	* \param  camera
	* \param  target
	*/
	AlignCameraState(Kouro::BaseCamera* camera, const Kouro::WorldTransform* target) : ICameraState(camera, target) {};

	void Enter() override;

	void Update(float deltaTime) override;

	void Exit() override;

private: // メンバ定数

};

