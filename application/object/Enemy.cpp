#include "Enemy.h"
#include "PlayerBullet.h"


///=============================================================================
///						初期化
void Enemy::Initialize() {
	//========================================
	// モデルを初期化
	model_ = std::make_unique<Object3d>();
	model_->Initialize(Object3dCommon::GetInstance());
	//========================================
	// モデルを読み込む
	ModelManager::GetInstance()->LoadModel("enemy/enemy.obj");
	model_->SetModel("enemy/enemy.obj");
	//========================================
	// 初期位置を設定
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	//========================================
	// 当たり判定との同期
	BaseObject::Initialize(worldTransform_->transform.translate, 1.0f);
}
///=============================================================================
///						描画
void Enemy::Update() {

	if (hp_ > 0) {
		//========================================
		
		// 移動処理
		worldTransform_->transform.translate.x += speed_ * direction_;

		// 端に到達したら方向を反転
		if (worldTransform_->transform.translate.x >= maxX_) {
			worldTransform_->transform.translate.x = maxX_;
			direction_ = -1;
		} else if (worldTransform_->transform.translate.x <= minX_) {
			worldTransform_->transform.translate.x = minX_;
			direction_ = 1;
		}	

		// ワールド変換の更新
		worldTransform_->UpdateMatrix();
		// モデルのワールド変換行列を更新
		model_->SetLocalMatrix(MakeIdentity4x4());

		// モデルの更新
		model_->Update();

		BulletUpdate();

		intervalCounter_ += 1.0f / 60.0f;

		if (kIntervalTiem <= intervalCounter_)
		{
			Fire();

			intervalCounter_ = fmod(intervalCounter_, kIntervalTiem);
		}

		//========================================
		// 当たり判定との同期
		BaseObject::Update(worldTransform_->transform.translate);
	}
}
///=============================================================================
///						描画
void Enemy::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {

	BulletDraw(viewProjection, directionalLight, pointLight, spotLight);

	//========================================
	// モデルの描画
	if (hp_ > 0) {
		model_->Draw(*worldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
	}
}

///=============================================================================
///						当たり判定
///--------------------------------------------------------------
///						接触開始処理
void Enemy::OnCollisionEnter(BaseObject* other) {
	if(dynamic_cast<PlayerBullet*>(other)) {
		--hp_;
	}
}

///--------------------------------------------------------------
///						接触継続処理
void Enemy::OnCollisionStay(BaseObject* other) {
}

///--------------------------------------------------------------
///						接触終了処理
void Enemy::OnCollisionExit(BaseObject* other) {

}

void Enemy::BulletUpdate()
{
	for (auto it = bullets_.begin(); it != bullets_.end(); ) 
	{
		if (!(*it)->GetIsActive()) {
			it = bullets_.erase(it);  // 非アクティブなら削除
		}
		else {
			it->get()->Update();

			++it;  // アクティブなら次へ
		}
	}

}

void Enemy::BulletDraw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	for (auto it = bullets_.begin(); it != bullets_.end(); ) 
	{
		it->get()->Draw(viewProjection, directionalLight, pointLight, spotLight);

		++it;
	}
}

void Enemy::Fire()
{
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();

		newBullet->Initialize(*worldTransform_.get(), target_->transform.translate);

		bullets_.push_back(std::move(newBullet));
}
