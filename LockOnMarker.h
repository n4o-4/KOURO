#pragma once
#include "Kouro.h"

class LockOnMarker {

public:

	void Initialize();
	void Show();
	void Hide();
	void Update();
	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

	void SetTargetPosition(const Vector3& position) { multilockOnWorldTransform_->transform.translate = position; }


private:


	//Multi lock on
	std::unique_ptr<Object3d> multiLockOn_ = nullptr;
	std::unique_ptr<WorldTransform> multilockOnWorldTransform_ = nullptr;
	bool isVisible_ = false;



};

