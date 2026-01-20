#include "TitleCamera.h"

#include "RotateCameraState.h"
#include "FollowCameraState.h"

void TitleCamera::Initialize()
{
    // カメラの初期オフセット設定
    offset_ = defaultOffset_;

    // 基底クラスの初期化
    BaseCamera::Initialize();

    //// 初期位置を設定
    //if (target_) {
    //    currentPosition_ = target_->transform.translate + offset_;
    //    viewProjection_->transform.translate = currentPosition_;
    //}
    //軽く傾ける

    worldTransform_->SetRotate({ 0.1f, 0.0f, 0.0f });
}

void TitleCamera::Update()
{
    if (state_)
    {
        state_->Update(1.0f / 60.0f);
    }

 //   // ターゲットが設定されていない場合はエラー
 //   if (target_ == nullptr)
 //   {
 //       assert(0);
 //   }

 //   if(isRotate_)
 //   {
 //       // カメラを回転させる
	//	viewProjection_->transform.rotate.y += 0.002f;
	//}

 //   if(isDeparture_)
 //   {
 //       // 吸収されるような演出
	//	viewProjection_->fovY += 0.007f;

 //       // ターゲットに追従する
	//	FollowTarget();
 //   }
 //   
 //   if (!isDeparture_)
 //   {
 //       // オフセットの計算
 //       CalculationRotate();

 //       // 回転の計算
 //       CalculationTranslate();
 //   }

    // 基底クラスの更新
    BaseCamera::Update();
}

Vector3 TitleCamera::CalculationOffset()
{

    return Vector3();
}

void TitleCamera::CalculationRotate()
{
}

void TitleCamera::CalculationTranslate()
{
   /* interTarget_ = Lerp(interTarget_, target_->transform.translate, easingFactor_);

    Vector3 offset = CalculationOffset();

    viewProjection_->transform.translate = interTarget_ + offset;*/
}

void TitleCamera::FollowTarget()
{
    //if (target_ == nullptr)
    //{
    //    assert(0);
    //}
    //
    //// target への方向を計算
    //Vector3 targetPos = target_->transform.translate;
    //Vector3 cameraPos = viewProjection_->transform.translate;

    //Vector3 direction = targetPos - cameraPos;

    //// 正規化
    //direction = Normalize(direction);

    //// yaw, pitch を求める
    //float yaw = std::atan2(direction.x, direction.z);
    //float pitch = std::atan2(-direction.y, std::sqrt(direction.x * direction.x + direction.z * direction.z));

    //// 回転を設定
    //viewProjection_->transform.rotate = { pitch, yaw, 0.0f };
}
