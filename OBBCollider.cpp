#include "OBBCollider.h"

void OBBCollider::Initialize(WorldTransform* worldTransform)
{
	// コライダーに関連付けるワールド変換情報を設定する
	colliderTransform_ = worldTransform;
	// OBBの初期化
	obb_.center = { 0.0f, 0.0f, 0.0f };
	obb_.halfSize = { 1.0f, 1.0f, 1.0f };
	obb_.axes[0] = { 0.0f, 0.0f, 0.0f }; // X軸
	obb_.axes[1] = { 0.0f, 0.0f, 0.0f }; // Y軸
	obb_.axes[2] = { 0.0f, 0.0f, 0.0f }; // Z軸
}

void OBBCollider::Update()
{
    // ワールド行列からスケール・回転・平行移動を抽出
    Matrix4x4 matWorld = colliderTransform_->matWorld_;

    // 中心（ワールド座標）＝ワールド行列の平行移動成分
    obb_.center = {
        matWorld.m[3][0],
        matWorld.m[3][1],
        matWorld.m[3][2]
    };

    // ワールド行列の各列ベクトルからローカル軸方向とスケールを取得
    Vector3 xAxis = { matWorld.m[0][0], matWorld.m[0][1], matWorld.m[0][2] };
    Vector3 yAxis = { matWorld.m[1][0], matWorld.m[1][1], matWorld.m[1][2] };
    Vector3 zAxis = { matWorld.m[2][0], matWorld.m[2][1], matWorld.m[2][2] };

    // 各軸の長さ（スケール）を計算
    float scaleX = Length(xAxis);
    float scaleY = Length(yAxis);
    float scaleZ = Length(zAxis);

    // 正規化して軸方向ベクトルにする
    obb_.axes[0] = Normalize(xAxis);  // X軸
    obb_.axes[1] = Normalize(yAxis);  // Y軸
    obb_.axes[2] = Normalize(zAxis);  // Z軸

    // ハーフサイズ（スケールの半分）
    obb_.halfSize = {
        scaleX * 0.5f,
        scaleY * 0.5f,
        scaleZ * 0.5f
    };
}
