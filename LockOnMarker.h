#pragma once
#include "Kouro.h"

class LockOnMarker {

public:

	void Initialize(size_t count);
	void Show(size_t index);
	void Hide();
	void Update();
	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

	void SetTargetPosition(size_t index, const Vector3& position);


private:


	//Multi lock on
	/*std::vector<std::unique_ptr<Object3d>> multiLockOns_;
	std::vector<std::unique_ptr<WorldTransform>> multiLockOnWorldTransforms_;
	bool isVisible_ = false;*/
	//
	struct MultiLockOn {
		std::unique_ptr<Object3d> lockOnUR;
		std::unique_ptr<Object3d> lockOnUL;
		std::unique_ptr<Object3d> lockOnDR;
		std::unique_ptr<Object3d> lockOnDL;

		std::unique_ptr<WorldTransform> worldTransformUR;
		std::unique_ptr<WorldTransform> worldTransformUL;
		std::unique_ptr<WorldTransform> worldTransformDR;
		std::unique_ptr<WorldTransform> worldTransformDL;

		bool isVisible = false;
		float rotation = 0.0f; 
	};
	std::vector<MultiLockOn> multiLockOns_;

};

