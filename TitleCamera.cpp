#include "TitleCamera.h"

///=============================================================================
///                        初期化
void TitleCamera::Initialize()
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
void TitleCamera::Update()
{
    // ターゲットが設定されていない場合はエラー
    if (target_ == nullptr)
    {
        assert(0);
    }

    if(isRotate_)
    {
		viewProjection_->transform.rotate.y += 0.002f;
	}

    if(isDeparture_)
    {
        viewProjection_->transform.rotate.y = 0.0f;

		FollowTarget();
    }
    
    if (!isDeparture_)
    {
        // オフセットの計算
        CalculationRotate();

        // 回転の計算
        CalculationTranslate();
    }
   
    

    // 基底クラスの更新
    BaseCamera::Update();
}

///=============================================================================
///                        オフセットの計算
Vector3 TitleCamera::CalculationOffset()
{
    Vector3 offset = offset_;

    Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->transform.rotate);

    offset = TransformNormal(offset, rotateMatrix);

    return offset;
}

///=============================================================================
///                        回転の計算
void TitleCamera::CalculationRotate()
{
   /* Vector3 rightStickVector = Input::GetInstance()->GetRightStick();

    Vector3 rotate = { -rightStickVector.y * rotateSpeed_, rightStickVector.x * rotateSpeed_ ,0.0f };

    destinationRotate += rotate;

    viewProjection_->transform.rotate = Lerp(viewProjection_->transform.rotate, destinationRotate, easingFactor_);

    float clampedX = std::clamp(viewProjection_->transform.rotate.x, -1.5f, 1.5f);
    if (clampedX != viewProjection_->transform.rotate.x) {
        destinationRotate.x = viewProjection_->transform.rotate.x;
    }

    viewProjection_->transform.rotate.x = std::clamp(viewProjection_->transform.rotate.x, -1.5f, 1.5f);*/
}

///=============================================================================
///                        位置の計算
void TitleCamera::CalculationTranslate()
{
    interTarget_ = Lerp(interTarget_, target_->transform.translate, easingFactor_);

    Vector3 offset = CalculationOffset();

    viewProjection_->transform.translate = interTarget_ + offset;
}

void TitleCamera::FollowTarget()
{
    if (target_ == nullptr)
    {
        assert(0);
    }
    
    // target への方向を計算
    Vector3 targetPos = target_->transform.translate;
    Vector3 cameraPos = viewProjection_->transform.translate;

    Vector3 direction = targetPos - cameraPos;

    // 正規化
    direction = Normalize(direction);

    // yaw, pitch を求める
    float yaw = std::atan2(direction.x, direction.z);
    float pitch = std::atan2(-direction.y, std::sqrt(direction.x * direction.x + direction.z * direction.z));

    // 回転を設定
    viewProjection_->transform.rotate = { pitch, yaw, 0.0f };
}
