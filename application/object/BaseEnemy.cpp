#include "BaseEnemy.h"
#include "PlayerMissile.h"
#include <cmath>
#include <algorithm>

///=============================================================================
///						初期化
void BaseEnemy::Initialize(Model* model) {
    //========================================
    // モデルを初期化
    model_ = std::make_unique<Object3d>();
    model_->Initialize(Object3dCommon::GetInstance());
    //========================================
    // モデルを読み込む
    model_->SetModel(model);
    //========================================
    // 初期位置を設定
    worldTransform_ = std::make_unique<WorldTransform>();
    worldTransform_->Initialize();
	/*spawnWorldTransform_ = std::make_unique<WorldTransform>();
	spawnWorldTransform_->Initialize();*/
    //========================================
    // 当たり判定との同期
    BaseObject::Initialize(worldTransform_->transform.translate, 1.0f);

    // 乱数生成器の初期化
    std::random_device rd;
    rng_ = std::mt19937(rd());


    // スポーン位置の初期化
    spawnPosition_ = worldTransform_->transform.translate;

    particleEmitter_ = std::make_unique<ParticleEmitter>();
    particleEmitter_->Initialize("missileSmoke");
    particleEmitter_->SetParticleCount(80);
    particleEmitter_->SetLifeTimeRange(ParticleManager::LifeTimeRange({ 1.0f,1.5f }));
    particleEmitter_->SetVelocityRange(ParticleManager::VelocityRange({ -10.0f,10.0f }, { -10.0f,10.0f }, { -10.0f,10.0f }));
}

///=============================================================================
///						描画
void BaseEnemy::Update() {
    if (IsAlive()) {

        // 弾の更新
        BulletUpdate();

        // ワールド変換の更新
        worldTransform_->UpdateMatrix();

        // モデルのローカル行列を更新
        model_->SetLocalMatrix(MakeIdentity4x4());
        // モデルの更新
        model_->Update();

        // パーティクルの位置を更新
        particleEmitter_->SetPosition(worldTransform_->transform.translate);

        //========================================
        // 当たり判定との同期
        BaseObject::Update(worldTransform_->transform.translate);
        if (worldTransform_->transform.translate.y < minY_) {
            worldTransform_->transform.translate.y = minY_;
        }
        if (worldTransform_->transform.translate.y > maxY_) {
            worldTransform_->transform.translate.y = maxY_;
        }
    }
}

///=============================================================================
///						描画
void BaseEnemy::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
    BulletDraw(viewProjection, directionalLight, pointLight, spotLight);

    //========================================
    // モデルの描画
    if (IsAlive()) {
        model_->Draw(*worldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
    }
}

///=============================================================================
///						当たり判定
///--------------------------------------------------------------
///						接触開始処理
void BaseEnemy::OnCollisionEnter(BaseObject* other) {
}

///--------------------------------------------------------------
///						接触継続処理
void BaseEnemy::OnCollisionStay(BaseObject* other) {
}

///--------------------------------------------------------------
///						接触終了処理
void BaseEnemy::OnCollisionExit(BaseObject* other) {
}

void BaseEnemy::BulletUpdate() {
    for (auto it = bullets_.begin(); it != bullets_.end(); ) {
        if (!(*it)->GetIsActive()) {
            it = bullets_.erase(it);  // 非アクティブなら削除
        }
        else {
            it->get()->Update();
            ++it;  // アクティブなら次へ
        }
    }
}

void BaseEnemy::BulletDraw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
    for (auto it = bullets_.begin(); it != bullets_.end(); ) {
        it->get()->Draw(viewProjection, directionalLight, pointLight, spotLight);
        ++it;
    }
}

void BaseEnemy::Fire() {
    if (target_) {
        std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
        newBullet->Initialize(*worldTransform_.get(), target_->transform.translate);
        bullets_.push_back(std::move(newBullet));
    }
}

void BaseEnemy::MoveToTarget()
{
    if (target_) {
        // 目標方向の計算
        Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
        Vector3 desiredDir = Normalize(toTarget);

        // 目標回転角度の計算
        float targetY = std::atan2(desiredDir.x, desiredDir.z);
        float currentY = worldTransform_->transform.rotate.y;

        // ステアリング制限回転
        float delta = targetY - currentY;
        delta = std::fmod(delta + std::numbers::pi, 2.0f * std::numbers::pi) - std::numbers::pi;

        float maxSteeringAngle = 0.05f; // フレームあたりの最大回転角
        delta = std::clamp(delta, -maxSteeringAngle, maxSteeringAngle);
        worldTransform_->transform.rotate.y += delta;

        //現在の回転方向に基づいて前進
        Vector3 forward = {
            sinf(worldTransform_->transform.rotate.y),
            0.0f,
            cosf(worldTransform_->transform.rotate.y)
        };

        // 速度補間(スムーズ)
        Vector3 targetVelocity = forward * speed_;
        velocity_ = Lerp(velocity_, targetVelocity, 0.1f);

        //位置の移動
        worldTransform_->transform.translate += velocity_;
    }
}

void BaseEnemy::RandomMove(float scale)
{
    directionChangeTimer_ += 1.0f / 60.0f;
    SetModelColor(Vector4{ 1.0f, 1.0f, 1.0f, 1.0f });

	float maxSteeringAngle = 0.05f; // 最大旋回角度

    // 移動目標方向の決定
    if (directionChangeTimer_ >= directionChangeInterval_) {
        Vector3 toSpawn = spawnPosition_ - worldTransform_->transform.translate;
        float distanceToSpawn = Length(toSpawn);

        Vector3 newDir;
        if (distanceToSpawn > wanderRadius_) {
            newDir = Normalize(toSpawn);
        } else {
            float angle = angleDist_(rng_);
            newDir = Vector3{ cosf(angle), 0.0f, sinf(angle) };
        }

        // 眺める目標角度の更新
        targetRotationY_ = std::atan2(newDir.x, newDir.z);

        directionChangeTimer_ = 0.0f;
    }

    // 回転加速度適用(自動車のようにスムーズに回転)
    float currentY = worldTransform_->transform.rotate.y;
    float delta = targetRotationY_ - currentY;
    delta = std::fmod(delta + std::numbers::pi, 2.0f * std::numbers::pi) - std::numbers::pi;
    delta = std::clamp(delta, -maxSteeringAngle, maxSteeringAngle);
    worldTransform_->transform.rotate.y += delta;

    float turnSpeed = 0.1f; // 回転感度
    rotationVelocityY_ += delta * turnSpeed;
    rotationVelocityY_ *= 0.85f; // 減速(摩擦)
    worldTransform_->transform.rotate.y += rotationVelocityY_;

    // 眺める方向を基準に移動(前進)
    Vector3 forward = {
        sinf(worldTransform_->transform.rotate.y),
        0.0f,
        cosf(worldTransform_->transform.rotate.y)
    };
    worldTransform_->transform.scale = { scale, scale, scale };
    velocity_ = forward * speed_;
    worldTransform_->transform.translate += velocity_;
}
// コンテキストベースの方向選択
Vector3 BaseEnemy::SelectDirection() {
    // 実装は省略（シンプルさを優先）
    return velocity_;
}