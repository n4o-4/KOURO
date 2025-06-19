#pragma once
#include "BaseCharacter.h"
class Enemy : public BaseCharacter
{
public: // 公開メンバ関数
	// 初期化処理
	void Initialize(Model* model) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) override;

	void SetPosition(const Vector3& position) {
		if (worldTransform_->useQuaternion_) {
			worldTransform_->quaternionTransform.translate = position;
		}
		else {
			worldTransform_->transform.translate = position;
		}
		worldTransform_->UpdateMatrix();
	}

private: // 非公開メンバ関数

	

};

