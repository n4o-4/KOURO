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
    
    UpdateCameraTranslate();

	UpdateCameraRotate();

    BaseCamera::Update();
}

void FollowCamera::UpdateCameraTranslate()
{
    // ターゲット位置を取得
    Vector3 targetPosition = target_->transform.translate + offset_;
    
    // 現在位置と目標位置の間を補間
    currentPosition_ = Lerp(currentPosition_, targetPosition, easingFactor_);
    
    // カメラ位置を更新
    viewProjection_->transform.translate = currentPosition_;
}

void FollowCamera::UpdateCameraRotate()
{
    Vector3 offset = {};

    Vector3 rightStickVector = Input::GetInstance()->GetRightStick();

    Vector3 rotate = { rightStickVector.y * 0.05f, rightStickVector.x * 0.05f,0.0f };

    viewProjection_->transform.rotate = viewProjection_->transform.rotate + rotate;

    // カメラの角度から回転行列を計算  
    Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->transform.rotate);

    // オフセットをカメラの回転に合わせて回転  
    offset = TransformNormal(offset_, rotateMatrix);

    // カメラの位置をターゲットの位置からオフセット分ずらす  
    viewProjection_->transform.translate = target_->transform.translate + offset;
}
