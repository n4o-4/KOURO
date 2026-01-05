#include "DebugCamera.h"  
#include <algorithm>

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
   Vector3 offSet = {0.0f,0.0f,0.0f};  

   if (Input::GetInstance()->mouseState.lZ != 0.0f)  
   {  
       float scrollAmount = static_cast<float>(Input::GetInstance()->mouseState.lZ) * kScrollSensitivity;

       // マウスホイールの回転量でオフセットを変更  
       offset.z += scrollAmount;

       offset.z = std::clamp(offset.z, kMinZoomDistance, kMaxZoomDistance);

       Input::GetInstance()->mouseState.lZ = static_cast<LONG>(0.0f);
   }  

    // カメラの角度から回転行列を計算  
    Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->transform.rotate);  

    // オフセットをカメラの回転に合わせて回転  
    offSet = TransformNormal(offset, rotateMatrix);  

    // カメラの位置をターゲットの位置からオフセット分ずらす  
    viewProjection_->transform.translate = targetTransform_->transform.translate + offSet;  

#ifdef _DEBUG

   if (ImGui::DragFloat3("camera.rotate", &viewProjection_->transform.rotate.x, 0.01f))
   {
       // カメラの角度から回転行列を計算  
       Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->transform.rotate);

       // オフセットをカメラの回転に合わせて回転  
       offSet = TransformNormal(offset, rotateMatrix);

       // カメラの位置をターゲットの位置からオフセット分ずらす  
       viewProjection_->transform.translate = targetTransform_->transform.translate + offSet;
   }

#endif

   // マウスの左ボタンが押されている間、カメラを回転
   if (Input::GetInstance()->PushMouseButton(Input::MouseButton::LEFT))  
   {  
       Vector3 rotate = {
    static_cast<float>(Input::GetInstance()->mouseState.lY) * kMouseRotateSensitivity,
    static_cast<float>(Input::GetInstance()->mouseState.lX) * kMouseRotateSensitivity,
    0.0f
       };

       viewProjection_->transform.rotate = viewProjection_->transform.rotate + rotate;  

      
   }  

   // カメラの角度から回転行列を計算  
   rotateMatrix = MakeRotateMatrix(viewProjection_->transform.rotate);

   // オフセットをカメラの回転に合わせて回転  
   offSet = TransformNormal(offset, rotateMatrix);

   // カメラの位置をターゲットの位置からオフセット分ずらす  
   viewProjection_->transform.translate = targetTransform_->transform.translate + offSet;

   // 汎用変数更新  
   BaseCamera::Update();  
}
