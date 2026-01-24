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
	RotateCameraState(Kouro::BaseCamera* camera, const Kouro::WorldTransform* target) : ICameraState(camera, target) {};

	/// \brief 開始処理
	void Enter() override;

	void Update(float deltaTime) override;

	void Exit() override;

private: // メンバ定数

	static constexpr float kRotateSpeed_ = 0.1f; //!< 回転速度
};

