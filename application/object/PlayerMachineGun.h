#pragma once
#include "BaseObject.h"
#include <Object3d.h>
#include "Kouro.h"

class PlayerMachineGun : public BaseObject {
public:
	// コンストラクタ
	PlayerMachineGun(const Vector3& position, const Vector3& velocity);

	// 更新処理
	void Update();

	// 描画処理
	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight,
		PointLight pointLight, SpotLight spotLight);

	// アクティブ状態の取得
	bool IsActive() const { return isActive_; }

	/// 【追加】BaseObject の仮想関数を実装
	void OnCollisionEnter(BaseObject* other) override;
	void OnCollisionStay(BaseObject* other) override;
	void OnCollisionExit(BaseObject* other) override;

private:
	// 移動速度
	Vector3 velocity_;

	// モデル
	std::unique_ptr<Object3d> model_;
	std::unique_ptr<WorldTransform> worldTransform_;

	// アクティブ状態
	bool isActive_ = true;

	// 弾の寿命
	int lifeTime_ = 0;

	std::unique_ptr<ParticleEmitter> particleEmitter_;
};
