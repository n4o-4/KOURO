#pragma once
#include "ICameraState.h"
class FollowCameraState : public ICameraState
{
public: // メンバ関数

	/**
	* \brief  コンストラクタ
	* \param  camera
	* \param  target
	*/
	FollowCameraState(BaseCamera* camera, const WorldTransform* target) : ICameraState(camera, target) {};


	void Enter() override;

	void Update(float deltaTime) override;

	void Exit() override;

private: // メンバ定数

};

