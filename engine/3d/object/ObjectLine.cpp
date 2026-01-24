#include "ObjectLine.h"

namespace Kouro
{
	void ObjectLine::Initialize(LineModel* model)
	{
		model_ = model;

		dxCommon_ = model_->GetdxCommon();

		// ライン用定数バッファの生成
		lineResource_ = dxCommon_->CreateBufferResource(sizeof(LineForGPU));
		lineResource_->Map(0, nullptr, reinterpret_cast<void**>(&lineData_));

		// スキャンエフェクト用定数バッファの生成
		scanEffectResource_ = dxCommon_->CreateBufferResource(sizeof(ScanEffectCB));
		scanEffectResource_->Map(0, nullptr, reinterpret_cast<void**>(&scanEffectData_));

		// スキャンエフェクト用定数バッファの初期化
		scanEffectData_->thickness = 0.05f;
		scanEffectData_->isRenderScanned = false;
		scanEffectData_->enableScan = false;

		// ライン色の初期化
		lineData_->color = { 1.0f,1.0f,1.0f,1.0f };
	}

	void ObjectLine::Draw(WorldTransform* worldTransform)
	{
		// モデルがセットされていない場合は描画しない
		if (model_ == nullptr)
		{
			assert(0);
		}

		// ワールド行列を転送
		lineData_->matWorld = worldTransform->matWorld_;

		dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, lineResource_.Get()->GetGPUVirtualAddress());
		dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, scanEffectResource_.Get()->GetGPUVirtualAddress());
		model_->Draw();
	}
}