#include "GroundTypeEnemy2.h"

void GroundTypeEnemy2::Initialize() {
	ModelManager::GetInstance()->LoadModel("enemy/bomb/bomb.obj");
	BaseEnemy::Initialize(ModelManager::GetInstance()->FindModel("enemy/bomb/bomb.obj"));
}

void GroundTypeEnemy2::Update() {
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
    }


    

    BaseEnemy::Update();
}

void GroundTypeEnemy2::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
    BaseEnemy::Draw(viewProjection, directionalLight, pointLight, spotLight);
}

void GroundTypeEnemy2::MoveToJump() {
}

void GroundTypeEnemy2::Attack() {
}

void GroundTypeEnemy2::OnCollisionEnter(BaseObject* other) {
}

void GroundTypeEnemy2::OnCollisionStay(BaseObject* other) {
}

void GroundTypeEnemy2::OnCollisionExit(BaseObject* other) {
}

void GroundTypeEnemy2::UpdateWanderState() {
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
            Vector3 randomDir = { cosf(angle), 0.0f, sinf(angle) };
            velocity_ = Normalize(randomDir) * speed_;
        }

        directionChangeTimer_ = 0.0f;
    }
}

void GroundTypeEnemy2::UpdateChaseState() {
    BaseEnemy::MoveToTarget();
}

void GroundTypeEnemy2::UpdateCombatState() {
}

void GroundTypeEnemy2::UpdateActionState() {
}
