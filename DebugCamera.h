#pragma once
#include "Input.h"
#include "BaseCamera.h"

class DebugCamera : public BaseCamera
{
public:

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	ViewProjection& GetViewProjection() { return *viewProjection_; }

private:
};

