#include "GroundTypeEnemy.h"
#include "PlayerBullet.h"
#include <cmath>
#include <algorithm>

void GroundTypeEnemy::Initialize() {
	BaseEnemy::Initialize();
}

void GroundTypeEnemy::Update() {
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

   
    /*intervalCounter_ += 1.0f / 60.0f;
    if (kIntervalTiem <= intervalCounter_ && currentState_ == ActionState::Combat) {
        Jump();
        intervalCounter_ = fmod(intervalCounter_, kIntervalTiem);
    }*/

    BaseEnemy::Update();
}

void GroundTypeEnemy::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
    BaseEnemy::Draw(viewProjection, directionalLight, pointLight, spotLight);
}

void GroundTypeEnemy::MoveToJump() {
    if (target_) {
    
        Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
        toTarget.y = 0.0f; 
        float distance = Length(toTarget);
        Vector3 direction = Normalize(toTarget);

        
        if (!isJumping_) {
            isJumping_ = true;
            jumpVelocity_ = 5.0f;  
            jumpTime_ = 0.0f;      
        }

       
        jumpTime_ += 1.0f / 60.0f;
        jumpVelocity_ -= gravity_ * jumpTime_;

       
        Vector3 moveDirection = direction * speed_;
        moveDirection.y = jumpVelocity_;

       
        worldTransform_->transform.translate = worldTransform_->transform.translate + moveDirection;

       
        if (worldTransform_->transform.translate.y <= groundHeight_) {
            worldTransform_->transform.translate.y = groundHeight_;
            isJumping_ = false;
        }
    
    
    }

}

void GroundTypeEnemy::OnCollisionEnter(BaseObject* other) {
    if (dynamic_cast<PlayerBullet*>(other)) {
        --hp_;
    }
}

void GroundTypeEnemy::OnCollisionStay(BaseObject* other) {
}

void GroundTypeEnemy::OnCollisionExit(BaseObject* other) {
}

void GroundTypeEnemy::UpdateWanderState() {
    BaseEnemy::RandomMove();
}

void GroundTypeEnemy::UpdateChaseState() {
    MoveToJump();
}

void GroundTypeEnemy::UpdateCombatState() {
    if (target_) {
        // ターゲットに向かうベクトルを計算
        Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
        float distance = Length(toTarget);
        Vector3 direction = Normalize(toTarget);

        // ストラッフィング（横移動）のベクトルを計算
        Vector3 strafeDir = { direction.z, 0.0f, -direction.x }; // 90度回転

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

void GroundTypeEnemy::UpdateActionState() {
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
