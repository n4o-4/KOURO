#include "DebugCamera.h"  
#include <algorithm>

namespace Kouro
{
    void DebugCamera::Initialize()
    {
        BaseCamera::Initialize();

        targetTransform_ = std::make_unique<WorldTransform>();
        targetTransform_->Initialize();

        // ターゲットからのオフセットを初期化  
        offset = { 0.0f, 0.0f, kDefaultCameraDistance };
    }

    void DebugCamera::Update()
    {
        /// デバッグカメラ特有の処理  

        // 計算用のオフセット  
        Vector3 offSet = { 0.0f,0.0f,0.0f };

		Vector3 mouseMove = Input::GetInstance()->GetMouseMove();

        if (mouseMove.z != 0.0f)
        {
            float scrollAmount = mouseMove.z * kScrollSensitivity;

            // マウスホイールの回転量でオフセットを変更  
            offset.z += scrollAmount;

            offset.z = std::clamp(offset.z, kMinZoomDistance, kMaxZoomDistance);

            mouseMove.z = 0.0f;
        }

        // カメラの角度から回転行列を計算  
        Matrix4x4 rotateMatrix = MakeRotateMatrix(worldTransform_->GetRotate());

        // オフセットをカメラの回転に合わせて回転  
        offSet = TransformNormal(offset, rotateMatrix);

        // カメラの位置をターゲットの位置からオフセット分ずらす  
        worldTransform_->SetTranslate(targetTransform_->GetTranslate() + offSet);

   // マウスの左ボタンが押されている間、カメラを回転
        if (Input::GetInstance()->PushMouseButton(Input::MouseButton::LEFT))
        {
            Vector3 rotate =
            {
                mouseMove.y * kMouseRotateSensitivity,
                mouseMove.x * kMouseRotateSensitivity,
                0.0f
            };

            worldTransform_->SetRotate(worldTransform_->GetRotate() + rotate);
        }

        // カメラの角度から回転行列を計算  
        rotateMatrix = MakeRotateMatrix(worldTransform_->GetRotate());

        // オフセットをカメラの回転に合わせて回転  
        offSet = TransformNormal(offset, rotateMatrix);

        // カメラの位置をターゲットの位置からオフセット分ずらす  
        worldTransform_->SetTranslate(worldTransform_->GetTranslate() + offset);

        // 汎用変数更新  
        BaseCamera::Update();
    }
}