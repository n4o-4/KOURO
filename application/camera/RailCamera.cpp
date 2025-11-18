#include "RailCamera.h"

void RailCamera::Initialize()
{
	BaseCamera::Initialize();

	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->useQuaternion_ = false;

	
	controlPoints_ =
	{
        {0.0f,0.0f,-20.0f},

        {0.0f,0.0f,20.0f},

        {0.0f,0.0f,480.0f},
        {15.0f,0.0f,495.0f},
        {20.0f,0.0,500.0f},

        {480.0f,0.0f,500.0f},
        {495.0f,0.0f,495.0f},
        {500.0f,0.0f,480.0f},

        {500.0f,0.0f,20.0f},
        {495.0f,0.0f,5.0f},
        {480.0f,0.0f,0.0f},

        {20.0f,0.0f,0.0f},
        {5.0f,0.0f,5.0f},
        {0.0f,0.0f,20.0f},

        {0.0f,0.0f,20.1f}, // 終了点
	};

	CreateArcLengthTable(100);
}

void RailCamera::Update()
{

//	 // 移動速度（曲線全体を kMoveTime 秒で移動）
//	float moveSpeed = arcLengthTable_.totalLength / kMoveTime;
//	static float distanceTravelled = 0.0f;
//
//	// 累積移動距離を更新
//	distanceTravelled += moveSpeed * kDeltaTime;
//
//	// ------------------------------
//	// 現在位置 t を累積距離から逆算
//	// ------------------------------
//	float t = 1.0f; // デフォルトは最後
//	for (int i = 1; i < arcLengthTable_.lengths.size(); ++i)
//	{
//		if (distanceTravelled <= arcLengthTable_.lengths[i])
//		{
//			float segmentLength = arcLengthTable_.lengths[i] - arcLengthTable_.lengths[i - 1];
//			float segmentT = (distanceTravelled - arcLengthTable_.lengths[i - 1]) / segmentLength;
//			t = (float)(i - 1 + segmentT) / (arcLengthTable_.lengths.size() - 1);
//			break;
//		}
//	}
//
//	// 現在位置
//	Vector3 position = CatmullRomPosition(controlPoints_, t);
//
//	// ------------------------------
//	// 次フレームのターゲット位置（進行方向）
//	// ------------------------------
//	float lookAheadDistance = moveSpeed * kDeltaTime * 2.0f; // 調整可能
//	float distanceAhead = distanceTravelled + lookAheadDistance;
//
//	float tNext = 1.0f;
//	for (int i = 1; i < arcLengthTable_.lengths.size(); ++i)
//	{
//		if (distanceAhead <= arcLengthTable_.lengths[i])
//		{
//			float segmentLength = arcLengthTable_.lengths[i] - arcLengthTable_.lengths[i - 1];
//			float segmentT = (distanceAhead - arcLengthTable_.lengths[i - 1]) / segmentLength;
//			tNext = (float)(i - 1 + segmentT) / (arcLengthTable_.lengths.size() - 1);
//			break;
//		}
//	}
//
//	Vector3 target = CatmullRomPosition(controlPoints_, tNext);
//
//	// ------------------------------
//	// forward ベクトルからオイラー角を計算
//	// ------------------------------
//	Vector3 forward = Normalize(target - position);
//	float yaw = std::atan2(forward.x, forward.z);
//	float pitch = std::asin(-forward.y);
//	float roll = 0.0f;
//
//	// ------------------------------
//	// カメラ変換を適用
//	// ------------------------------
//	worldTransform_->transform.translate = position;
//	worldTransform_->transform.rotate = { pitch, yaw, roll };
//	worldTransform_->UpdateMatrix();
//
//	viewProjection_->matView_ = Inverse(worldTransform_->matWorld_);
//
//	viewProjection_->transform = worldTransform_->transform;
//	viewProjection_->transform.rotate = worldTransform_->transform.rotate;
//
//#ifdef _DEBUG
//
//	ImGui::Begin("RailCamera_Transform");
//	ImGui::DragFloat3("Position", &viewProjection_->transform.translate.x, 0.01f);
//	ImGui::DragFloat3("Rotate", &viewProjection_->transform.rotate.x, 0.01f);
//	ImGui::DragFloat3("Scale", &viewProjection_->transform.scale.x, 0.01f);
//	ImGui::End();
//
//#endif
//
//	BaseCamera::Update();

    // 移動速度（曲線全体を kMoveTime 秒で移動）
    float moveSpeed = arcLengthTable_.totalLength / kMoveTime;

    if (!isMove_)
    {
        return;
    }

    // 累積移動距離を更新
    distanceTravelled += moveSpeed * kDeltaTime;

    if (distanceTravelled >= arcLengthTable_.totalLength)
    {
        distanceTravelled = 0.0f;
    }

    // ------------------------------
    // 現在位置 t を累積距離から逆算
    // ------------------------------
    float t = 1.0f; // デフォルトは最後
    for (int i = 1; i < arcLengthTable_.lengths.size(); ++i)
    {
        if (distanceTravelled <= arcLengthTable_.lengths[i])
        {
            float segmentLength = arcLengthTable_.lengths[i] - arcLengthTable_.lengths[i - 1];
            float segmentT = (distanceTravelled - arcLengthTable_.lengths[i - 1]) / segmentLength;
            t = (float)(i - 1 + segmentT) / (arcLengthTable_.lengths.size() - 1);
            break;
        }
    }

    // 現在位置
    Vector3 position = CatmullRomPosition(controlPoints_, t);

    // ------------------------------
    // 次フレームのターゲット位置（進行方向）
    // ------------------------------
    float lookAheadDistance = moveSpeed * kDeltaTime * 2.0f; // 調整可能
    float distanceAhead = distanceTravelled + lookAheadDistance;

    float tNext = 1.0f;
    for (int i = 1; i < arcLengthTable_.lengths.size(); ++i)
    {
        if (distanceAhead <= arcLengthTable_.lengths[i])
        {
            float segmentLength = arcLengthTable_.lengths[i] - arcLengthTable_.lengths[i - 1];
            float segmentT = (distanceAhead - arcLengthTable_.lengths[i - 1]) / segmentLength;
            tNext = (float)(i - 1 + segmentT) / (arcLengthTable_.lengths.size() - 1);
            break;
        }
    }

    Vector3 target = CatmullRomPosition(controlPoints_, tNext);

    // ------------------------------
    // forward ベクトルからオイラー角を計算
    // ------------------------------
    Vector3 forward = Normalize(target - position);

    // 進行方向とは逆向き（後方）に10.0fオフセット
    const float cameraOffset = -25.0f;
    Vector3 offset = forward * cameraOffset;
    position += offset;

    // 回転の計算（forwardはオフセット前でもOK）
    float yaw = std::atan2(forward.x, forward.z);
    float pitch = std::asin(-forward.y);
    float roll = 0.0f;

    // ------------------------------
    // カメラ変換を適用
    // ------------------------------
    worldTransform_->transform.translate = position;
    worldTransform_->transform.rotate = { pitch, yaw, roll };
    worldTransform_->UpdateMatrix();

    viewProjection_->matView_ = Inverse(worldTransform_->matWorld_);

    viewProjection_->transform = worldTransform_->transform;
    viewProjection_->transform.rotate = worldTransform_->transform.rotate;

#ifdef _DEBUG
    ImGui::Begin("RailCamera_Transform");
    ImGui::DragFloat3("Position", &viewProjection_->transform.translate.x, 0.01f);
    ImGui::DragFloat3("Rotate", &viewProjection_->transform.rotate.x, 0.01f);
    ImGui::DragFloat3("Scale", &viewProjection_->transform.scale.x, 0.01f);
    ImGui::End();
#endif

    BaseCamera::Update();
}

void RailCamera::CreateArcLengthTable(int samplePerSegment)
{
	int numSegments = static_cast<int>(controlPoints_.size()) - 3;
	int totalSamples = numSegments * samplePerSegment;

	arcLengthTable_.lengths.resize(totalSamples + 1);
	arcLengthTable_.lengths[0] = 0.0f;
	arcLengthTable_.totalLength = 0.0f;

	Vector3 prevPoint = CatmullRomPosition(controlPoints_, 0.0f);

	for (int i = 1; i <= totalSamples; ++i)
	{
		float t = static_cast<float>(i) / totalSamples; // 0〜1 正規化
		Vector3 currPoint = CatmullRomPosition(controlPoints_, t);

		float segmentLength = Length(currPoint - prevPoint);
		arcLengthTable_.lengths[i] = arcLengthTable_.lengths[i - 1] + segmentLength;

		prevPoint = currPoint;
	}

	arcLengthTable_.totalLength = arcLengthTable_.lengths.back();
}
