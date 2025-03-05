#include "FollowCamera.h"

void FollowCamera::Initialize()
{
    // カメラの初期オフセット設定
    offset_ = { 0.0f, 3.0f, -30.0f };
    
    // 基底クラスの初期化
    BaseCamera::Initialize();
    
    // 初期位置を設定
    if (target_) {
        currentPosition_ = target_->transform.translate + offset_;
        viewProjection_->transform.translate = currentPosition_;
    }
	//軽く傾ける
	viewProjection_->transform.rotate = { 0.1f, 0.0f, 0.0f };
}

void FollowCamera::Update()
{
    if (target_ == nullptr)
    {
        assert(0);
    }

    // ターゲット位置を取得
    Vector3 targetPosition = target_->transform.translate + offset_;
    
    // 現在位置と目標位置の間を補間
    currentPosition_ = Lerp(currentPosition_, targetPosition, easingFactor_);
    
    // カメラ位置を更新
    viewProjection_->transform.translate = currentPosition_;

    BaseCamera::Update();
}
