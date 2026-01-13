#pragma once
#include "ICameraState.h"
class RotateCameraState : public ICameraState
{
public: // メンバ関数

	/**
	* \brief  コンストラクタ
	* \param  camera
	* \param  target
	*/
	RotateCameraState(BaseCamera* camera, const WorldTransform* target) : ICameraState(camera, target) {};


	void Enter() override;

	void Update(float deltaTime) override;

	void Exit() override;

private: // メンバ定数

	static constexpr float kRotateSpeed_ = 0.1f; //!< 回転速度
};

