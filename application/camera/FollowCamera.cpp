#define _USE_MATH_DEFINES
#define NOMINMAX
#include <algorithm>
#include <cmath>
#include "MyMath.h"
#include "FollowCamera.h"

///=============================================================================
///                        初期化
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

    // 高度によるFOV演出
    UpdateHeightFOVEffect();

    // 基底クラスの更新
    BaseCamera::Update();
}
///=============================================================================
///                        前方向ベクトルを取得
Vector3 FollowCamera::GetForwardDirection() const {
    // カメラの回転行列から前方向ベクトルを計算
    Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->transform.rotate);
    Vector3 forward = { 0.0f, 0.0f, 1.0f }; // デフォルトの前方向
    return TransformNormal(forward, rotateMatrix);
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
}
///=============================================================================
///                        位置の計算
void FollowCamera::CalculationTranslate()
{
    interTarget_ = Lerp(interTarget_, target_->transform.translate, easingFactor_);

    Vector3 offset = CalculationOffset();

    viewProjection_->transform.translate = interTarget_ + offset;
}
///=============================================================================
///                        高度によるFOV演出
void FollowCamera::UpdateHeightFOVEffect()
{
    // ターゲットが設定されていない場合は処理しない
    if (!target_) return;

    // プレイヤーの高度を取得
    float playerHeight = target_->transform.translate.y;
    
    // 高度に基づいたFOV調整（Y座標が0.0以上の場合）
    if (playerHeight >= 0.0f) {
        // 高度に応じてFOV増加量を計算
        // 高度が高いほどFOVを広げる（最大でmaxHeightForFOV_まで考慮）
        float heightFactor = std::min(playerHeight / maxHeightForFOV_, 1.0f);
        
        // 高度に応じたFOV増加を計算
        float fovIncrease = heightFOVIncrease_ * heightFactor;
        targetFOV_ = baseFOV_ + fovIncrease;
    } 
    else {
        // 地上（Y < 0.0）では基本FOVに戻す
        targetFOV_ = baseFOV_;
    }
    
    // 現在のFOVを目標値へ滑らかに補間
    currentFOV_ = currentFOV_ + (targetFOV_ - currentFOV_) * fovLerpSpeed_;

    // フォールバックチェック（極端な値にならないよう制限）
    currentFOV_ = std::max(baseFOV_, std::min(currentFOV_, baseFOV_ + heightFOVIncrease_));
    
    // FOVをカメラに適用（ラジアンへの変換が必要な場合）
    viewProjection_->fovY = currentFOV_ * (3.14159f / 180.0f);
    
    // 現在の高度を保存（必要に応じて）
    prevPlayerHeight_ = playerHeight;
}