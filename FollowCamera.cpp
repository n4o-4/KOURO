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

    CalculationRotate();

    CalculationTranslate();

    BaseCamera::Update();
}

Vector3 FollowCamera::CalculationOffset()
{
    Vector3 offset = offset_;

    Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->transform.rotate);

    offset = TransformNormal(offset, rotateMatrix);

    return offset;
}

void FollowCamera::CalculationRotate()
{
    Vector3 rightStickVector = Input::GetInstance()->GetRightStick();

    Vector3 rotate = { rightStickVector.y * 0.1f, rightStickVector.x * 0.1f,0.0f };

    destinationRotate += rotate;

    viewProjection_->transform.rotate = Lerp(viewProjection_->transform.rotate, destinationRotate, easingFactor_);
}

void FollowCamera::CalculationTranslate()
{
    interTarget_ = Lerp(interTarget_, target_->transform.translate, easingFactor_);

    Vector3 offset = CalculationOffset();

    viewProjection_->transform.translate = interTarget_ + offset;
}