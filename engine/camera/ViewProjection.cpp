#include "ViewProjection.h"
#include "WinApp.h"

void ViewProjection::Initialize()
{
	// ビュープロジェクション行列用の定数バッファを生成
	viewProjectionResource_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBufferDataViewProjection));
	viewProjectionResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&viewProjectionData_));

	// 初期化
	transform.scale = { 1.0f,1.0f,1.0f };
	transform.rotate = { 0.0f,0.0f,0.0f };
	transform.translate = { 0.0f, 0.0f, kDefaultCameraDistance };
	fovY = kDefaultFovY;
	aspectRation = static_cast<float>(WinApp::kClientWidth) / static_cast<float>(WinApp::kClientHeight);
	nearClip = kDefaultNearClip;
	farClip = kDefaultFarClip;
	viewProjectionData_->matViewProjection = MakeIdentity4x4();
}

void ViewProjection::Update()
{
	// ワールド行列の計算
	worldPosition_ = transform.translate;
	matWorld_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate + traslateOffset);

	// ビュー行列の計算
	matView_ = Inverse(matWorld_);

	// プロジェクション行列の計算
	matProjection_ = MakePerspectiveFovMatrix(fovY, aspectRation, nearClip, farClip);

	viewProjectionData_->worldPosition = worldPosition_;

	// ビュー・プロジェクション行列の計算
	viewProjectionData_->matViewProjection = Multiply(matView_,matProjection_);
}

void ViewProjection::TransferMatrix()
{
	// ビュー・プロジェクション行列の計算
	viewProjectionData_->matViewProjection = Multiply(matView_, matProjection_);
}
