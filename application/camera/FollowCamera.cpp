#define _USE_MATH_DEFINES
#define NOMINMAX
#include <algorithm>
#include <cmath>
#include "MyMath.h"
#include "FollowCamera.h"
#include "Input.h"

void FollowCamera::Initialize()
{
    // カメラの初期オフセット設定
    offset_ = defaultOffset_;

    // 基底クラスの初期化
    BaseCamera::Initialize();

    // 初期位置を設定
    if (target_) {
        currentPosition_ = target_->transform.translate + offset_;
        worldTransform_->SetTranslate(currentPosition_);
    }
    //軽く傾ける
    worldTransform_->SetRotate({ 0.1f, 0.0f, 0.0f });
}

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

Vector3 FollowCamera::CalculationOffset()
{
	// オフセットの初期化
    Vector3 offset = offset_;

	// 回転行列の作成
    Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransform_->GetRotate());

	// オフセットに回転を適用
    offset = TransformNormal(offset, rotateMatrix);

	// 計算したオフセットを返す
    return offset;
}

void FollowCamera::CalculationRotate()
{
	// スティックの入力取得
    Vector3 rightStickVector = Input::GetInstance()->GetRightStick();

	// 回転速度
    Vector3 rotate = { -rightStickVector.y * rotateSpeed_, rightStickVector.x * rotateSpeed_ ,0.0f };

	// 目的の回転角度を計算
    destinationRotate += rotate;

	// 回転をイージングで補間
    worldTransform_->SetRotate(Lerp(worldTransform_->GetRotate(), destinationRotate, easingFactor_));

	// X軸の回転角度を制限
    float clampedX = std::clamp(worldTransform_->GetRotate().x, -1.5f, 1.5f);
    if (clampedX != worldTransform_->GetRotate().x)
    {
        destinationRotate.x = worldTransform_->GetRotate().x;
    }
}

void FollowCamera::CalculationTranslate()
{
	// ターゲットの位置をイージングで補間
    interTarget_ = Lerp(interTarget_, target_->transform.translate, easingFactor_);

	// オフセットの計算
    Vector3 offset = CalculationOffset();

	// カメラの位置を設定
    //viewProjection_->transform.translate = interTarget_ + offset;
}