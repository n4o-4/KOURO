#include "PlayerMachineGun.h"
#include "Enemy.h"

PlayerMachineGun::PlayerMachineGun(const Vector3 &position, const Vector3 &velocity) {
	model_ = std::make_unique<Object3d>();
	model_->Initialize(Object3dCommon::GetInstance());

	ModelManager::GetInstance()->LoadModel("player.obj");
	model_->SetModel("player.obj");

	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->transform.translate = position;
	worldTransform_->transform.scale = {0.8f, 0.8f, 0.8f}; // マシンガンの弾は小さめ
	velocity_ = velocity;

	BaseObject::Initialize(worldTransform_->transform.translate, 5.5f); // 当たり判定のサイズ

	ParticleManager::GetInstance()->CreateParticleGroup("missileSmoke", "Resources/circle.png", ParticleManager::ParticleType::Normal);
	particleEmitter_ = std::make_unique<ParticleEmitter>();
	particleEmitter_->Initialize("missileSmoke");
	particleEmitter_->SetParticleCount(10); // デフォルト発生数
	particleEmitter_->SetFrequency(0.04f);	// より高頻度で発生させる
	particleEmitter_->SetLifeTimeRange(ParticleManager::LifeTimeRange({0.01f, 0.01f}));
	particleEmitter_->SetStartColorRange(ParticleManager::ColorRange({0.66f, 0.85f}, {0.64f, 0.83f}, {0.18f, 0.21f}, {1.0f, 1.0f}));
}

void PlayerMachineGun::Update() {
	lifeTime_++;

	// 一定時間で消滅
	if (lifeTime_ > 120) { // 約2秒
		isActive_ = false;
	}

	// 移動処理
	worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;
	model_->SetLocalMatrix(MakeIdentity4x4()); // ローカル行列を単位行列に
	worldTransform_->UpdateMatrix();

	BaseObject::Update(worldTransform_->transform.translate);
}

void PlayerMachineGun::Draw(ViewProjection viewProjection, DirectionalLight directionalLight,
							PointLight pointLight, SpotLight spotLight) {
	if (isActive_) {
		model_->Draw(*worldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
	}
}

std::unique_ptr<PlayerMachineGun> PlayerMachineGun::Shoot(
	const Vector3 &playerPosition,
	FollowCamera *followCamera,
	Vector3 &outRecoilVelocity,
	float &outShakeIntensity) {

	// プレイヤーの向いている方向を取得（カメラの方向を基準にする）
	Vector3 forward = followCamera->GetForwardDirection(); // カメラの正面ベクトルを直接取得
	// もしGetForwardDirectionが正規化されたベクトルを返さない場合はここで正規化
	// forward = Normalize(forward);

	// ランダムなばらつきの大きさを計算
	float randomSpreadX = ((rand() % 100) / 100.0f - 0.5f) * kSpread;
	float randomSpreadY = ((rand() % 100) / 100.0f - 0.5f) * kSpread;

	// カメラ（プレイヤーの視点）の右方向と上方向を取得
	Vector3 right = followCamera->GetRightDirection();
	Vector3 up = followCamera->GetUpDirection();
	// もしこれらの関数が正規化されたベクトルを返さない場合はここで正規化
	// right = Normalize(right);
	// up = Normalize(up);

	// ばらつきをプレイヤーのローカル座標系で適用
	Vector3 spreadOffset = right * randomSpreadX + up * randomSpreadY;
	Vector3 bulletDirection = forward + spreadOffset;

	// 最終的な弾の速度ベクトル
	Vector3 bulletVelocity = Normalize(bulletDirection) * kBulletSpeed;

	// 反動の計算
	Vector3 recoilDirection = -forward; // 反動は元のforward方向の反対
	outRecoilVelocity = recoilDirection * kRecoilStrength;

	// 画面の揺れを設定
	outShakeIntensity = kInitialShakeIntensity;

	// マシンガン弾を生成して返す
	return std::make_unique<PlayerMachineGun>(playerPosition, bulletVelocity);
}

///=============================================================================
///                        当たり判定の実装
///=============================================================================
void PlayerMachineGun::OnCollisionEnter(BaseObject *other) {
	// 敵接触
	if (Enemy *enemy = dynamic_cast<Enemy *>(other)) {
		//---------------------------------------
		// 弾を消す
		isActive_ = false;

		particleEmitter_->Emit();
	}
}

void PlayerMachineGun::OnCollisionStay(BaseObject *other) {
	// 今回は特に処理なし
}

void PlayerMachineGun::OnCollisionExit(BaseObject *other) {
	// 今回は特に処理なし
}
