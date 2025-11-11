#include "ObjectLine.h"

void ObjectLine::Initialize(LineModel* model)
{
	model_ = model;

	dxCommon_ = model_->GetdxCommon();

	lineResource_ = dxCommon_->CreateBufferResource(sizeof(LineForGPU));
	lineResource_->Map(0, nullptr, reinterpret_cast<void**>(&lineData_));

	scanEffectResource_ = dxCommon_->CreateBufferResource(sizeof(ScanEffectCB));
	scanEffectResource_->Map(0, nullptr, reinterpret_cast<void**>(&scanEffectData_));

	scanEffectData_->thickness = 0.05f;

	scanEffectData_->isRenderScanned = false;

	scanEffectData_->enableScan = false;

	lineData_->color = { 1.0f,1.0f,1.0f,1.0f };
}

void ObjectLine::Draw(WorldTransform* worldTransform)
{
	if (model_ == nullptr)
	{
		assert(0, "モデルがセットされていません");
	}

	lineData_->matWorld = worldTransform->matWorld_;

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, lineResource_.Get()->GetGPUVirtualAddress());

	dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, scanEffectResource_.Get()->GetGPUVirtualAddress());

	model_->Draw();
}

