#pragma once
#include "BaseCamera.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "FollowCamera.h"
#include "RailCamera.h"
#include "TitleCamera.h"

// \brief CameraManager
// 複数カメラを管理するクラス。
// デバッグカメラや追従カメラなどを保持し、更新・切り替え・ターゲット設定を行う。

class CameraManager
{
public: /// メンバ関数	

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// アクティブカメラの取得
	BaseCamera* GetActiveCamera() { return activeCamera_; }

	void SetFollowCameraTarget(WorldTransform* target) { followCamera_->SetTarget(target); }

	FollowCamera* GetFollowCamera() { return followCamera_.get(); }

	void SetActiveCamera(BaseCamera* camera) { activeCamera_ = camera; }

	void CamerasClear();

public:

	void DrawDebugUI();

private:

	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;

	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	std::list<BaseCamera*> cameras_;

	BaseCamera* activeCamera_ = nullptr;
};

