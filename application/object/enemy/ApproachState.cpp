#include "ApproachState.h"

#include "RandomMoveState.h"
#include "LissajousMoveState.h"

#include "Enemy.h"
#include "Player.h"

void ApproachState::OnEnter(Enemy* enemy)
{
    Kouro::Vector3 offsetDir = TransformNormal({ 0.0f, 0.0f, -kApproachRadius }, enemy->GetWorldTransform()->GetWorldMatrix());

	Kouro::Matrix4x4 matWorld = enemy->GetWorldTransform()->GetWorldMatrix();

    // カメラの位置を取得
    Kouro::Vector3 parentPos = { matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2] };

    // ワールド座標での敵位置 = カメラ位置 + 回転されたオフセット方向
    Kouro::Vector3 worldPos = parentPos + offsetDir;

    // 敵のTransform更新
    Kouro::WorldTransform* enemyTransform = enemy->GetWorldTransform();
    enemyTransform->SetTranslate(worldPos);
    enemyTransform->UpdateMatrix();
}

void ApproachState::Update(Enemy* enemy)
{
    Kouro::WorldTransform* enemyTransform = enemy->GetWorldTransform();

    Kouro::Vector3 startPos = { 0.0f, 0.0f, -kApproachRadius }; // 出現位置
    Kouro::Vector3 goalPos = enemy->GetGoalOffset();         // 目標位置

    // 正規化ベクトル
    Kouro::Vector3 startDir = Kouro::Normalize(startPos);
    Kouro::Vector3 goalDir = Kouro::Normalize(goalPos);

    // 大円の角度
    float dotProd = Dot(startDir, goalDir);
    dotProd = std::clamp(dotProd, -1.0f, 1.0f); // 安全
    float theta = acosf(dotProd);

    // 進行度 t (0->1)
    angle_ += kApproachSpeed * (1.0f / 60.0f); // deg/frame
    if (angle_ > kEndAngle) angle_ = kEndAngle;
    float t = angle_ / kEndAngle; // 0→1

    Kouro::Vector3 pos;
    if (theta > 0.001f) {
        float sinTheta = sinf(theta);
        pos = (startDir * sinf((1 - t) * theta) + goalDir * sinf(t * theta)) / sinTheta;
    }
    else {
        pos = goalDir; // ほぼ同じ場合
    }

    // スケール（半径）を kApproachRadius に
    pos = pos * kApproachRadius;

    // Y方向のうねり
    pos.y += kApproachHeight * sinf(t * 3.14159f); // 半波

    enemyTransform->SetTranslate(pos);
    enemyTransform->SetRotate({ enemyTransform->GetRotate().x, enemyTransform->GetRotate().y + 0.01f, enemyTransform->GetRotate().z });
    enemyTransform->UpdateMatrix();

    if (angle_ >= kEndAngle)
    {
        std::unique_ptr<RandomMoveState> newState = std::make_unique<RandomMoveState>();

        enemy->ChangeState(std::move(newState));
    }
}

void ApproachState::OnExit(Enemy* enemy)
{
}
