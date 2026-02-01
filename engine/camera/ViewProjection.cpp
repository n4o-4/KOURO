#include "ViewProjection.h"
#include "WinApp.h"

namespace Kouro
{
	void ViewProjection::Initialize()
	{
		// ビュープロジェクション行列用の定数バッファを生成
		viewProjectionResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBufferDataViewProjection));
		viewProjectionResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&viewProjectionData_));

		fovY_ = kDefaultFovY;
		aspectRation_ = static_cast<float>(WinApp::kClientWidth) / static_cast<float>(WinApp::kClientHeight);
		nearClip_ = kDefaultNearClip;
		farClip_ = kDefaultFarClip;
		viewProjectionData_->matViewProjection = MakeIdentity4x4();
	}

	void ViewProjection::Update(const WorldTransform& worldTransform)
	{
		// 最終的なワールド行列を使用
		const Matrix4x4& matWorld = worldTransform.matWorld_;

		// 平行移動（カメラの位置）
		Vector3 worldPos = { matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2] };

		// ワールド座標を保存
		worldPosition_ = worldPos;

		// 回転を取り出す場合（行列からオイラー角へ変換）
		Vector3 worldRot = GetEulerFromMatrix(matWorld);

		// シェイク補正を追加
		Vector3 finalPos = worldPos + shakeOffset_;

		matWorld_ = MakeAffineMatrix({ 1,1,1 }, worldRot, finalPos);

		// View行列を生成（カメラは逆変換）
		matView_ = Inverse(MakeAffineMatrix({ 1,1,1 }, worldRot, finalPos));

		// Projection行列
		matProjection_ = MakePerspectiveFovMatrix(fovY_, aspectRation_, nearClip_, farClip_);

		viewProjectionData_->matViewProjection = Multiply(matView_, matProjection_);
	}

	void ViewProjection::TransferMatrix()
	{
		// ビュー・プロジェクション行列の計算
		viewProjectionData_->matViewProjection = Multiply(matView_, matProjection_);
	}
}