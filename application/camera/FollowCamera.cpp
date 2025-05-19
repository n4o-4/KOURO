#define _USE_MATH_DEFINES
#define NOMINMAX
#include <algorithm>
#include <cmath>
#include "MyMath.h"
#include "FollowCamera.h"
#include "Input.h"
#include "BaseEnemy.h"

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

    // 高度によるFOV演出
    UpdateHeightFOVEffect();
    
    // マシンガン発射時のカメラ演出を更新
    UpdateFiringCameraEffect();

    // ミサイル発射時のカメラ演出を更新
    UpdateMissileFiringCameraEffect();

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
///                        カメラからの右方向ベクトルを取得
Vector3 FollowCamera::GetRightDirection() const {
    // カメラの回転行列から右方向ベクトルを計算
    Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->transform.rotate);
    Vector3 right = { 1.0f, 0.0f, 0.0f }; // デフォルトの右方向
    return TransformNormal(right, rotateMatrix);
}

///=============================================================================
///                        カメラからの上方向ベクトルを取得
Vector3 FollowCamera::GetUpDirection() const {
    // カメラの回転行列から上方向ベクトルを計算
    Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->transform.rotate);
    Vector3 up = { 0.0f, 1.0f, 0.0f }; // デフォルトの上方向
    return TransformNormal(up, rotateMatrix);
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
   /* Vector2 rightStickVector = Input::GetInstance()->GetRightStick();

    Vector3 rotate = { -rightStickVector.y * rotateSpeed_, rightStickVector.x * rotateSpeed_ ,0.0f };

    destinationRotate += rotate;

    viewProjection_->transform.rotate = Lerp(viewProjection_->transform.rotate, destinationRotate, easingFactor_);

    float clampedX = std::clamp(viewProjection_->transform.rotate.x, -1.5f, 1.5f);
    if (clampedX != viewProjection_->transform.rotate.x) {
        destinationRotate.x = viewProjection_->transform.rotate.x;
    }

    viewProjection_->transform.rotate.x = std::clamp(viewProjection_->transform.rotate.x, -1.5f, 1.5f);*/

    /// 右スティック傾きを取得
    Vector2 rightStickVector = Input::GetInstance()->GetRightStick();

    Vector2 rotate = { -rightStickVector.y * rotateSpeed_, rightStickVector.x * rotateSpeed_ };

	destinationRotate = { destinationRotate.x + rotate.x, destinationRotate.y + rotate.y,destinationRotate.z };

    viewProjection_->transform.rotate = Lerp(viewProjection_->transform.rotate, destinationRotate, easingFactor_);

    // 回転Xを範囲制限
    float clampedX = std::clamp(viewProjection_->transform.rotate.x, -0.8f, 1.5f);
    if (clampedX != viewProjection_->transform.rotate.x) {
        destinationRotate.x = clampedX;
    }
    viewProjection_->transform.rotate.x = clampedX;
}

///=============================================================================
///                        位置の計算
void FollowCamera::CalculationTranslate()
{
    interTarget_ = Lerp(interTarget_, target_->transform.translate, easingFactor_);

    Vector3 offset = CalculationOffset();

    viewProjection_->transform.translate = interTarget_ + offset;

    /// カメラの角度の修正が必要無ければ
    if (viewProjection_->transform.translate.y >= 0.1f)
    {
        return;
    }

    // カメラの高さが0.1以下なら、下方向（上向き）への回転を制限
    while(viewProjection_->transform.translate.y <= 0.1f) {
        // 下向きすぎないように制限（上を向かせすぎない）
        //destinationRotate.x = std::max(destinationRotate.x, 0.1f);

        viewProjection_->transform.rotate.x += 0.001f;

        interTarget_ = Lerp(interTarget_, target_->transform.translate, easingFactor_);

        Vector3 offset = CalculationOffset();

        viewProjection_->transform.translate = interTarget_ + offset;
    }

    // 目標角度を修正
    destinationRotate.x = viewProjection_->transform.rotate.x;
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

///=============================================================================
///                        マシンガン発射時のカメラ演出を更新
void FollowCamera::UpdateFiringCameraEffect(){
    //ミサイル発射中はマシンガン発射中の演出を無効化
    if (isMissileFiring_) return;

    Vector3 targetOffset = defaultOffset_;
    
    // マシンガン発射中はカメラを近づける
    // RTボタンが押されている間だけ近づける
    if (Input::GetInstance()->PushKey(DIK_J) ||
    Input::GetInstance()->PushGamePadButton(Input::GamePadButton::LEFT_SHOULDER)) {
        // Z方向（距離）のみ調整して近づける
        targetOffset.z = defaultOffset_.z * firingOffsetFactor_;
    }
    
    // 現在のオフセットを目標値に滑らかに補間
    offset_ = Lerp(offset_, targetOffset, firingLerpSpeed_);
}

///=============================================================================
///                        ミサイル発射時のカメラ演出を更新
void FollowCamera::UpdateMissileFiringCameraEffect(){
    Vector3 targetOffset = defaultOffset_;
    
    // ミサイル発射ボタンが押された時に発射状態を開始
    if (Input::GetInstance()->PushKey(DIK_SPACE) ||
        Input::GetInstance()->PushGamePadButton(Input::GamePadButton::RIGHT_SHOULDER)) {
        isMissileFiring_ = true;
        missileFireTimer_ = 0.0f;  // タイマーをリセット
    }
    
    // ミサイル発射中の処理（数秒間継続）
    if (isMissileFiring_) {
        // カメラを遠ざける
        targetOffset.z = defaultOffset_.z * missileFiringOffsetFactor_;
        
        // タイマーを進める（60FPSを想定）
        missileFireTimer_ += 1.0f / 60.0f;
        
        // 持続時間が経過したら演出を終了
        if (missileFireTimer_ >= missileFireDuration_) {
            isMissileFiring_ = false;
        }
    }
    
    // 現在のオフセットを目標値に滑らかに補間
    offset_ = Lerp(offset_, targetOffset, missileFiringLerpSpeed_);
}