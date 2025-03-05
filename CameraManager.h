#pragma once
#include "BaseCamera.h"
#include "Camera.h"
#include "DebugCamera.h"

class CameraManager
{
public: /// メンバ関数	

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// アクティブカメラの取得
	BaseCamera* GetActiveCamera() { return activeCamera_; }

public:

	bool useDebugCamera_ = false;

private:
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;

	std::list<BaseCamera*> cameras_;

	BaseCamera* activeCamera_ = nullptr;
};

