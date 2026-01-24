#include "RailFollower.h"

void RailFollower::Initialize(const Rail& rail,float moveTime)
{
	// 弧長データのポインタを取得
	arcLengthTable_ = rail.GetArcLengthTable();

	// コントロールポイントのリストのポインタを取得
	controlPoints_ = rail.GetControlPoints();

	moveTime_ = moveTime;

	worldTransform_.Initialize();
	worldTransform_.useQuaternion_ = false;
}

void RailFollower::Update(float deltaTime)
{
	// 1秒あたりの移動量を求める
	float speed = arcLengthTable_.totalLength / moveTime_;

	// フレーム当たりの移動量に変換
	speed *= deltaTime;

	// 累計移動距離にフレーム当たりの移動量を加算
	distanceTravelled_ += speed;

	// もし累計移動距離が
	if (distanceTravelled_ > arcLengthTable_.totalLength)
	{
		distanceTravelled_ -= arcLengthTable_.totalLength;
	}

	// 現在位置 t を累積距離から逆算
	float t = kEndOfCurveT;
	for (int i = 1; i < arcLengthTable_.lengths.size(); ++i)
	{
		if (distanceTravelled_ <= arcLengthTable_.lengths[i])
		{
			float segmentLength = arcLengthTable_.lengths[i] - arcLengthTable_.lengths[i - kPreviousIndexOffset];
			float segmentT = (distanceTravelled_ - arcLengthTable_.lengths[i - kPreviousIndexOffset]) / segmentLength;
			t = (float)(i - kPreviousIndexOffset + segmentT) / (arcLengthTable_.lengths.size() - 1);
			break;
		}
	}

	// 現在位置
	Kouro::Vector3 position = CatmullRomPosition(controlPoints_, t);

	// 次フレームのターゲット位置（進行方向）
	float distanceAhead = distanceTravelled_ + lookAheadDistance_;

	float tNext = 1.0f;
	for (int i = 1; i < arcLengthTable_.lengths.size(); ++i)
	{
		if (distanceAhead <= arcLengthTable_.lengths[i])
		{
			float segmentLength = arcLengthTable_.lengths[i] - arcLengthTable_.lengths[i - kPreviousIndexOffset];
			float segmentT = (distanceAhead - arcLengthTable_.lengths[i - kPreviousIndexOffset]) / segmentLength;
			tNext = (float)(i - kPreviousIndexOffset + segmentT) / (arcLengthTable_.lengths.size() - 1);
			break;
		}
	}

	// 現在の向き(進行方向)
	Kouro::Vector3 target = CatmullRomPosition(controlPoints_, tNext);

	
	// forward ベクトルからオイラー角を計算
	Kouro::Vector3 forward = Normalize(target - position);
	float yaw = std::atan2(forward.x, forward.z);
	float pitch = std::asin(-forward.y);
	float roll = 0.0f;

	// ------------------------------
	// カメラ変換を適用
	// ------------------------------
	worldTransform_.transform.translate = position;
	worldTransform_.transform.rotate = { pitch, yaw, roll };

	worldTransform_.UpdateMatrix();
}