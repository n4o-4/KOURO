#define _USE_MATH_DEFINES
#define NOMINMAX
#include <algorithm>
#include <cmath>
#include "MyMath.h"
#include "FollowCamera.h"
#include "Input.h"

///=============================================================================
///                        初期化
void FollowCamera::Initialize()
{
    // カメラの初期オフセット設定
    offset_ = defaultOffset_;

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

///=============================================================================
///                        更新
void FollowCamera::Update()
{
    // ターゲットが設定されていない場合はエラー
    if (target_ == nullptr)
    {
        assert(0);
    }
    
    // オフセットの計算
    CalculationRotate();

    // 回転の計算
    CalculationTranslate();

   

    // 基底クラスの更新
    BaseCamera::Update();
}

///=============================================================================
///                        オフセットの計算
Vector3 FollowCamera::CalculationOffset()
{
    Vector3 offset = offset_;

    Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->transform.rotate);

    offset = TransformNormal(offset, rotateMatrix);

    return offset;
}

///=============================================================================
///                        回転の計算
void FollowCamera::CalculationRotate()
{
    Vector3 rightStickVector = Input::GetInstance()->GetRightStick();

    Vector3 rotate = { -rightStickVector.y * rotateSpeed_, rightStickVector.x * rotateSpeed_ ,0.0f };

    destinationRotate += rotate;

    viewProjection_->transform.rotate = Lerp(viewProjection_->transform.rotate, destinationRotate, easingFactor_);

    float clampedX = std::clamp(viewProjection_->transform.rotate.x, -1.5f, 1.5f);
    if (clampedX != viewProjection_->transform.rotate.x) {
        destinationRotate.x = viewProjection_->transform.rotate.x;
    }

    viewProjection_->transform.rotate.x = std::clamp(viewProjection_->transform.rotate.x, -1.5f, 1.5f);
}

///=============================================================================
///                        位置の計算
void FollowCamera::CalculationTranslate()
{
    interTarget_ = Lerp(interTarget_, target_->transform.translate, easingFactor_);

    Vector3 offset = CalculationOffset();

    viewProjection_->transform.translate = interTarget_ + offset;
}