#pragma once
#include "BaseCharacter.h"
class Enemy : public BaseCharacter, public AABBCollider
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

	// \brief  OnCollisionEnter 衝突開始時の処理

	void OnCollisionEnter(BaseCollider* other) override;

	// \brief  OnCollisionStay 衝突中の処理

	void OnCollisionStay(BaseCollider* other) override;

	// \brief  OnCollisionExit 衝突終了時の処理

	void OnCollisionExit(BaseCollider* other) override;

};

