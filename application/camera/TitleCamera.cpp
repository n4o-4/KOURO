#include "TitleCamera.h"

#include "RotateCameraState.h"
#include "TransitionCameraState.h"

void TitleCamera::Initialize()
{
    // カメラの初期オフセット設定
    offset_ = defaultOffset_;

    // 基底クラスの初期化
    BaseCamera::Initialize();

    worldTransform_->SetRotate({ 0.1f, 0.0f, 0.0f });
}

void TitleCamera::Update()
{
    if (state_)
    {
        state_->Update(1.0f / 60.0f);
    }

    // 基底クラスの更新
    BaseCamera::Update();
}

void TitleCamera::CalculationRotate()
{
}

void TitleCamera::CalculationTranslate()
{
}

void TitleCamera::FollowTarget()
{
}
