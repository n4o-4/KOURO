#include "Enemy.h"
#include "PlayerMissile.h"
#include <cmath>
#include <algorithm>

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
    
    // 乱数生成器の初期化
    std::random_device rd;
    rng_ = std::mt19937(rd());
    
    // スポーン位置の初期化
    spawnPosition_ = worldTransform_->transform.translate;
}

///=============================================================================
///						描画
void Enemy::Update() {
    if (hp_ > 0) {
        // 状態の更新
        UpdateActionState();
        
        // 現在の状態に応じた行動
        switch (currentState_) {
        case ActionState::Wander:
            UpdateWanderState();
            break;
        case ActionState::Chase:
            UpdateChaseState();
            break;
        case ActionState::Combat:
            UpdateCombatState();
            break;
        }

        // 弾の発射処理
        intervalCounter_ += 1.0f / 60.0f;
        if (kIntervalTiem <= intervalCounter_ && currentState_ == ActionState::Combat) {
            Fire();
            intervalCounter_ = fmod(intervalCounter_, kIntervalTiem);
        }
        
        // 弾の更新
        BulletUpdate();
        
        // ワールド変換の更新
        worldTransform_->UpdateMatrix();
        // モデルのローカル行列を更新
        model_->SetLocalMatrix(MakeIdentity4x4());
        // モデルの更新
        model_->Update();

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
    if(dynamic_cast<PlayerMissile*>(other)) {
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

void Enemy::BulletUpdate() {
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

void Enemy::BulletDraw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
    for (auto it = bullets_.begin(); it != bullets_.end(); ) {
        it->get()->Draw(viewProjection, directionalLight, pointLight, spotLight);
        ++it;
    }
}

void Enemy::Fire() {
    if (target_) {
        std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
        newBullet->Initialize(*worldTransform_.get(), target_->transform.translate);
        bullets_.push_back(std::move(newBullet));
    }
}

// 状態の更新
void Enemy::UpdateActionState() {
    // タイマーの更新
    stateTimer_ += 1.0f / 60.0f;
    
    // ターゲットが存在しない場合は徘徊
    if (!target_) {
        currentState_ = ActionState::Wander;
        return;
    }
    
    // ターゲットとの距離を計算
    Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
    float distance = Length(toTarget);
    
    // 距離に基づいて状態を変更
    ActionState newState = currentState_; // デフォルトは現在の状態を維持
    
    if (distance > chaseDistance_) {
        // プレイヤーが遠い場合は徘徊
        newState = ActionState::Wander;
    } else if (distance > combatDistance_) {
        // 追跡範囲内なら追跡
        newState = ActionState::Chase;
    } else {
        // 戦闘範囲内なら戦闘
        newState = ActionState::Combat;
    }
    
    // 状態が変化した場合、タイマーをリセット
    if (newState != currentState_) {
        currentState_ = newState;
        stateTimer_ = 0.0f;
    }
}

// 徘徊状態の更新
void Enemy::UpdateWanderState() {
    // 方向変更タイマーの更新
    directionChangeTimer_ += 1.0f / 60.0f;
    
    // 定期的に方向を変更
    if (directionChangeTimer_ >= directionChangeInterval_) {
        // スポーン地点に戻る方向と、ランダムな方向を混ぜる
        Vector3 toSpawn = spawnPosition_ - worldTransform_->transform.translate;
        float distanceToSpawn = Length(toSpawn);
        
        // スポーン地点から遠すぎる場合はスポーン地点に戻る傾向を強める
        float spawnWeight = std::min(distanceToSpawn / wanderRadius_, 0.8f);
        
        if (distanceToSpawn > wanderRadius_) {
            // スポーン地点に戻る方向を優先
            velocity_ = Normalize(toSpawn) * speed_;
        } else {
            // ランダムな方向を選択
            float angle = angleDist_(rng_);
            Vector3 randomDir = {cosf(angle), 0.0f, sinf(angle)};
            velocity_ = Normalize(randomDir) * speed_;
        }
        
        directionChangeTimer_ = 0.0f;
    }
    
    // 位置を更新
    worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;
    
    // 敵の向きを進行方向に合わせる
    if (Length(velocity_) > 0.01f) {
        float targetRotationY = std::atan2(velocity_.x, velocity_.z);
        worldTransform_->transform.rotate.y = targetRotationY;
    }
}

// 追跡状態の更新
void Enemy::UpdateChaseState() {
    if (target_) {
        // ターゲットに向かうベクトルを計算
        Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
        float distance = Length(toTarget);
        
        // 距離が十分にある場合のみ追跡
        if (distance > combatDistance_) {
            Vector3 direction = Normalize(toTarget);
            velocity_ = direction * speed_;
            
            // 位置を更新
            worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;
            
            // 敵の向きを進行方向に合わせる
            float targetRotationY = std::atan2(direction.x, direction.z);
            worldTransform_->transform.rotate.y = targetRotationY;
        }
    }
}

// 戦闘状態の更新
void Enemy::UpdateCombatState() {
    if (target_) {
        // ターゲットに向かうベクトルを計算
        Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
        float distance = Length(toTarget);
        Vector3 direction = Normalize(toTarget);
        
        // ストラッフィング（横移動）のベクトルを計算
        Vector3 strafeDir = {direction.z, 0.0f, -direction.x}; // 90度回転
        
        // 時間とともに方向を変えるための係数（-1から1の範囲）
        float strafeFactor = sinf(stateTimer_ * 0.5f);
        
        // 距離に基づいて行動を決定
        Vector3 moveDirection;
        
        if (distance < safeDistance_) {
            // 近すぎる場合、後退
            //moveDirection = -direction * 0.7f + strafeDir * strafeFactor * 0.3f;
			moveDirection.x = -direction.x * 0.7f + strafeDir.x * strafeFactor * 0.3f;
			moveDirection.y = 0.0f;
			moveDirection.z = -direction.z * 0.7f + strafeDir.z * strafeFactor * 0.3f;
        } else if (distance > combatDistance_) {
            // 遠すぎる場合、接近
            moveDirection = direction * 0.7f + strafeDir * strafeFactor * 0.3f;
        } else {
            // 適切な距離の場合、ストラッフィングのみ
            moveDirection = strafeDir * strafeFactor;
        }
        
        // 速度を更新
        velocity_ = Normalize(moveDirection) * speed_ * 0.8f;
        
        // 位置を更新
        worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;
        
        // 常にターゲットの方を向く
        float targetRotationY = std::atan2(direction.x, direction.z);
        worldTransform_->transform.rotate.y = targetRotationY;
    }
}

// コンテキストベースの方向選択
Vector3 Enemy::SelectDirection() {
    // 実装は省略（シンプルさを優先）
    return velocity_;
}
