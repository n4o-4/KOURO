#include "ApproachState.h"
#include "Enemy.h"
#include "Player.h"

void ApproachState::Enter(Enemy* enemy, const WorldTransform* worldTransform)
{
    // カメラのTransformを親として登録
    parent_ = worldTransform;

    enemy->GetWorldTransform()->SetParent(worldTransform);

    Vector3 offsetDir = TransformNormal({ 0.0f, 0.0f, -kApproachRadius }, parent_->matWorld_);

    // カメラの位置を取得
    Vector3 parentPos = { parent_->matWorld_.m[3][0], parent_->matWorld_.m[3][1], parent_->matWorld_.m[3][2] };

    // ワールド座標での敵位置 = カメラ位置 + 回転されたオフセット方向
    Vector3 worldPos = parentPos + offsetDir;

    // 敵のTransform更新
    WorldTransform* enemyTransform = enemy->GetWorldTransform();
    enemyTransform->transform.translate = worldPos;
    enemyTransform->UpdateMatrix();
}

void ApproachState::Update(Enemy* enemy)
{
    //Vector3 goalOffset = enemy->GetGoalOffset();

    //WorldTransform* enemyTransform = enemy->GetWorldTransform();

    //// 経過時間に応じて角度を進める
    //angle_ += kApproachSpeed * 1.0f / 60.0f;  // deltaTime_ は Enemy側で渡す想定
    //if (angle_ > kEndAngle) { angle_ = kEndAngle; }

    //// 角度をラジアンに変換
    //float rad = ToRadians(angle_);

    //// 円弧を描くように移動（XZ平面）
    //enemyTransform->transform.translate.x = kApproachRadius * sinf(rad);
    //enemyTransform->transform.translate.z = -kApproachRadius * cosf(rad);

    //// 上下方向に少しうねり（sin波）
    //enemyTransform->transform.translate.y = kApproachHeight * sinf(rad * 0.5f);

    //// 更新
    //enemyTransform->UpdateMatrix();

    //// 前方に到達したら次のステートへ
    //if (angle_ >= kEndAngle) {
    //    // enemy->ChangeState(...);
    //}

    //ver2.0
    //WorldTransform* enemyTransform = enemy->GetWorldTransform();

    //Vector3 startPos = {0.0f,0.0f,-40.0f}; // 出現位置（Enterで決定済み）
    //Vector3 goalPos = enemy->GetGoalOffset();             // 目標位置

    //angle_ += kApproachSpeed * (1.0f / 60.0f);
    //if (angle_ > kEndAngle) angle_ = kEndAngle;

    //float rad = ToRadians(angle_);

    //// XZ平面の方向ベクトル
    //Vector3 horizontal = { goalPos.x - startPos.x, 0.0f, goalPos.z - startPos.z };
    //float distanceXZ =Length(horizontal);
    //if (distanceXZ < 0.001f) distanceXZ = 0.001f;
    //horizontal = Normalize(horizontal);

    //// 弧の中心 = startとgoalの中間 + 上方向オフセット
    //Vector3 midPos = {
    //    (startPos.x + goalPos.x) / 2.0f,
    //    (startPos.y + goalPos.y) / 2.0f + kApproachHeight,
    //    (startPos.z + goalPos.z) / 2.0f
    //};

    //// horizontal に垂直なXZ方向ベクトル
    //Vector3 ortho = { -horizontal.z, 0.0f, horizontal.x };

    //// 弧に沿った位置
    //Vector3 offset;
    //offset.x = midPos.x + (-horizontal.x * kApproachRadius * cosf(rad)) + (ortho.x * kApproachRadius * sinf(rad));
    //offset.y = midPos.y + kApproachHeight * sinf(rad * 0.5f);
    //offset.z = midPos.z + (-horizontal.z * kApproachRadius * cosf(rad)) + (ortho.z * kApproachRadius * sinf(rad));

    //enemyTransform->transform.translate = offset;
    //enemyTransform->UpdateMatrix();

    //if (angle_ >= kEndAngle) {
    //    // enemy->ChangeState(...);
    //}

    WorldTransform* enemyTransform = enemy->GetWorldTransform();

    Vector3 startPos = { 0.0f, 0.0f, -kApproachRadius }; // 出現位置
    Vector3 goalPos = enemy->GetGoalOffset();         // 目標位置

    // 正規化ベクトル
    Vector3 startDir = Normalize(startPos);
    Vector3 goalDir = Normalize(goalPos);

    // 大円の角度
    float dotProd = Dot(startDir, goalDir);
    dotProd = std::clamp(dotProd, -1.0f, 1.0f); // 安全
    float theta = acosf(dotProd);

    // 進行度 t (0->1)
    angle_ += kApproachSpeed * (1.0f / 60.0f); // deg/frame
    if (angle_ > kEndAngle) angle_ = kEndAngle;
    float t = angle_ / kEndAngle; // 0→1

    Vector3 pos;
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

    enemyTransform->transform.translate = pos;
    enemyTransform->transform.rotate.y += 0.01f;
    enemyTransform->UpdateMatrix();

    if (angle_ >= kEndAngle) {
        // enemy->ChangeState(...);
    }
}

void ApproachState::Exit(Enemy* enemy)
{
}
