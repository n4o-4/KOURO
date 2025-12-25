#include "Camera.h"
#include "WinApp.h"

std::unique_ptr<Camera> Camera::instance = nullptr;

Camera* Camera::GetInstance()
{
	if (instance == nullptr) {
		instance = std::make_unique<Camera>();
	}

	return instance.get();
}

void Camera::Initialize()
{
	// 基底クラスの初期化
	BaseCamera::Initialize();
}

void Camera::Finalize()
{

	instance.reset();


}

void Camera::Update()
{
	// 基底クラスの更新
	BaseCamera::Update();
}
