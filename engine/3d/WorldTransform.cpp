#include "WorldTransform.h"

void WorldTransform::Initialize()
{

	transformResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBufferDataWorldTransform));

	// データを書き込むためのアドレスを取得
	transformResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformData_));

	transform.scale = { 1.0f,1.0f,1.0f };

	transform.rotate = { 0.0f,0.0f,0.0f };

	transform.translate = { 0.0f,0.0f,0.0f };

	quaternionTransform.scale = { 1.0f,1.0f,1.0f };

	quaternionTransform.rotate = { 0.0f,0.0f,0.0f ,1.0f };

	quaternionTransform.translate = { 0.0f,0.0f,0.0f };
}

void WorldTransform::UpdateMatrix()
{
	if (useQuaternion_)
	{
		quaternionTransform.rotate = qNormalize(quaternionTransform.rotate);

		// クォータニオンの差を誤差付きで比較
		const float epsilon = 1e-5f;
		bool hasChanged =
			std::abs(preRotate_.x - quaternionTransform.rotate.x) > epsilon ||
			std::abs(preRotate_.y - quaternionTransform.rotate.y) > epsilon ||
			std::abs(preRotate_.z - quaternionTransform.rotate.z) > epsilon ||
			std::abs(preRotate_.w - quaternionTransform.rotate.w) > epsilon;

		if (hasChanged)
		{
			// 正規化
			Quaternion q1 = qNormalize(preRotate_);
			Quaternion q2 = qNormalize(quaternionTransform.rotate);

			// クォータニオンの内積
			float dot = qDot(q1, q2);

			// クォータニオンは -q と q が同じ回転を表すため、dot が負なら反転して最近傍の回転を選ぶ
			if (dot < 0.0f) {
				dot = -dot;
			}

			// [-1, 1] にクランプ（数値誤差対策）
			dot = std::clamp(dot, -1.0f, 1.0f);

			// なす角（ラジアン）を求める（acos の戻り値は半分の角度なので 2倍）
			float angleRad = std::acos(dot) * 2.0f;

			// 角度（度数法）も表示すると便利
			float angleDeg = angleRad * (180.0f / static_cast<float>(std::numbers::pi));

			// ImGui 表示
			ImGui::Text("回転差角度: %.3f rad / %.2f deg", angleRad, angleDeg);
		}

		matWorld_ = MakeAffineMatrixforQuater(quaternionTransform.scale, quaternionTransform.rotate, quaternionTransform.translate);
	}
	else
	{
		matWorld_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	}

	matWorldInverse_ = Inverse(matWorld_);

	transformData_->matWorld = matWorld_;

	transformData_->matWorldInverse = matWorldInverse_;

	if (useQuaternion_)
	{
		preRotate_ = quaternionTransform.rotate;
	}
}

void WorldTransform::TransferMatrix()
{
	matWorldInverse_ = Inverse(matWorld_);

	transformData_->matWorld = matWorld_;

	transformData_->matWorldInverse = matWorldInverse_;
}
