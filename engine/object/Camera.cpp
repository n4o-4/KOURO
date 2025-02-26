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
	viewProjection.Initialize();

	viewProjection.transform.translate = { 0.0f,0.0f,-15.0f };

	viewProjection.Update();

	worldMatrix = MakeAffineMatrix(viewProjection.transform.scale, viewProjection.transform.rotate, viewProjection.transform.translate);
}

void Camera::Finalize()
{

	instance.reset();


}

void Camera::Update()
{
	worldMatrix = MakeAffineMatrix(viewProjection.transform.scale, viewProjection.transform.rotate, viewProjection.transform.translate);

	viewProjection.Update();
}
