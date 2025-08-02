#pragma once
#include "BaseCamera.h"

#include "MyMath.h"

#include "WorldTransform.h"

class RailCamera : public BaseCamera
{
public:

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	//getter
	WorldTransform* GetWorldTransform() { return worldTransform_.get(); }

private:

	std::unique_ptr<WorldTransform> worldTransform_ = nullptr; //!< ワールド変形情報

	std::vector<Vector3> controlPoints_; //!< カメラの制御点

	const float kMoveTime = 120.0f;

	float moveTimer_ = 0.0f; //!< カメラの移動時間
};

