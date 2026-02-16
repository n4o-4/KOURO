#include "TitleCamera.h"

#include "RotateCameraState.h"
#include "AlignCameraState.h"
#include "FollowCameraState.h"
#include "LookAtPlayerState.h"

void TitleCamera::Initialize()
{
    // カメラの初期オフセット設定
    offset_ = defaultOffset_;

    // 基底クラスの初期化
    BaseCamera::Initialize();

    worldTransform_->SetRotate({ 0.1f, 0.0f, 0.0f });

    ChangeState(std::make_unique<RotateCameraState>(this, target_));
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

void TitleCamera::RequestChangeState(State state)
{
    switch (state)
    {
    case State::Rotate:
        ChangeState(std::make_unique<RotateCameraState>(this, target_));
        break;
    case State::Align:
        ChangeState(std::make_unique<AlignCameraState>(this, target_));
        break;
	case State::Follow:
		ChangeState(std::make_unique<FollowCameraState>(this, target_));
		break;
	case State::LookAtPlayer:
		ChangeState(std::make_unique<LookAtPlayerState>(this, target_));
		break;
    default:
        break;
	}
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
