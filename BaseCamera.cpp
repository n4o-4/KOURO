#include "BaseCamera.h"

void BaseCamera::Initialize()
{
	// viewProjectionÇÃèâä˙âª
	viewProjection.Initialize();

	viewProjection.transform.translate = { 0.0f,0.0f,-15.0f };

	viewProjection.Update();

	worldMatrix = MakeAffineMatrix(viewProjection.transform.scale, viewProjection.transform.rotate, viewProjection.transform.translate);
}

void BaseCamera::Update()
{
	worldMatrix = MakeAffineMatrix(viewProjection.transform.scale, viewProjection.transform.rotate, viewProjection.transform.translate);

	viewProjection.Update();
}
