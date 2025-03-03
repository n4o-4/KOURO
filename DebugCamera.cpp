#include "DebugCamera.h"

void DebugCamera::Initialize()
{
	BaseCamera::Initialize();
}

void DebugCamera::Update()
{
	// デバッグカメラ特有の処理

	if (Input::GetInstance()->PushMouseButton(Input::MouseButton::LEFT))
	{

		Vector3 rotate = { static_cast<float>(Input::GetInstance()->mouseState.lY) / 1000.0f, static_cast<float>(Input::GetInstance()->mouseState.lX) / 1000.0f,0.0f };
		
		viewProjection_->transform.rotate= viewProjection_->transform.rotate + rotate;
	}


	// 汎用変数更新
	BaseCamera::Update();
}
