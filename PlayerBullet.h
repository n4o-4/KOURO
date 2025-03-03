#pragma once
#include "Vectors.h"
#include "Object3d.h"
#include "WorldTransform.h"

class PlayerBullet {
public:
	PlayerBullet(Vector3 position, Vector3 velocity, Vector3 scale = { 1.0f, 1.0f, 1.0f }, Vector3 rotate = { 0.0f, 0.0f, 0.0f });

	void Initialize(Vector3 position, Vector3 velocity, Vector3 scale, Vector3 rotate); // 初期化関数

	void Update();
	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

	bool IsActive() const { return isActive_; }

	Vector3 GetPosition() const { return objectTransform_->transform.translate; }

	void SetScale(Vector3 scale) { objectTransform_->transform.scale = scale; }
	void SetRotation(Vector3 rotation) { objectTransform_->transform.rotate = rotation; }
	void SetPosition(Vector3 position) { objectTransform_->transform.translate = position; }

private:
	Vector3 velocity_;
	bool isActive_ = true;

	std::unique_ptr<Object3d> object3d_;
	std::unique_ptr<WorldTransform> objectTransform_; // `objectTransform_` に変更
};
