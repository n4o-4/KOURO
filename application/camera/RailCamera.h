#pragma once
#include "BaseCamera.h"

#include "MyMath.h"

class RailCamera : public BaseCamera
{
public:

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

private:

	std::vector<Vector3> controlPoints_; //!< カメラの制御点

	const float kMoveTime = 10.0f;

	float moveTimer_ = 0.0f; //!< カメラの移動時間

};

