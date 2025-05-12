#include "PostEffect.h"

void PostEffect::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
{
	dxCommon_ = dxCommon;

	srvManager_ = srvManager;
}

void PostEffect::Finalize()
{
	for (auto it = activeEffects_.begin(); it != activeEffects_.end(); ++it) {
		(*it).second->effect->Reset();
	}

	activeEffects_.clear();
}

void PostEffect::Update()
{
	for (auto it = activeEffects_.begin(); it != activeEffects_.end(); ++it) {

		(*it).second->effect->Update();

	}

#ifdef _DEBUG

	DrawImGui();

#endif
}

void PostEffect::Draw()
{
    // 書き込む方のインデックス
	uint32_t targetIndex = dxCommon_->GetRenderTargetIndex();
	// 読み込む方のインデックス
	uint32_t resourceIndex = dxCommon_->GetRenderResourceIndex();

	dxCommon_->GetCommandList()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto it = activeEffects_.begin(); it != activeEffects_.end(); ++it) {

		// 
		(*it).second->effect->Draw(targetIndex,resourceIndex);

		// 現在書き込んだ方を読み込み用に変換
		D3D12_RESOURCE_BARRIER barrier1{};
		barrier1.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier1.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier1.Transition.pResource = dxCommon_->GetRenderTextureResources()[targetIndex].Get();
		barrier1.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier1.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier1);

		// 今読み込んだ方を描画用に変換
		D3D12_RESOURCE_BARRIER barrier2{};
		barrier2.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier2.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier2.Transition.pResource = dxCommon_->GetRenderTextureResources()[resourceIndex].Get();
		barrier2.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		barrier2.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

		dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier2);

		std::swap(targetIndex, resourceIndex);
	}

	dxCommon_->SetRenderTargetIndex(targetIndex);
	dxCommon_->SetRenderResourceIndex(resourceIndex);
}

void PostEffect::ApplyEffect(std::string name, EffectType type)
{
	std::unique_ptr<PostEffect::ActiveEffect> newEffect = std::make_unique<PostEffect::ActiveEffect>();

	if (type == EffectType::Grayscale)
	{
		newEffect->effect = std::make_unique<Grayscale>();
		newEffect->effect->Initialize(dxCommon_, srvManager_);
	}
	else if (type == EffectType::Vignette)
	{
		newEffect->effect = std::make_unique<Vignette>();
		newEffect->effect->Initialize(dxCommon_, srvManager_);
	}
	else if (type == EffectType::BoxFilter)
	{
		newEffect->effect = std::make_unique<BoxFilter>();
		newEffect->effect->Initialize(dxCommon_, srvManager_);
	}
	else if (type == EffectType::GaussianFilter)
	{
		newEffect->effect = std::make_unique<GaussianFilter>();
		newEffect->effect->Initialize(dxCommon_, srvManager_);
	}
	else if (type == EffectType::LuminanceBasedOutline)
	{
		newEffect->effect = std::make_unique<LuminanceBasedOutline>();
		newEffect->effect->Initialize(dxCommon_, srvManager_);
	}
	else if (type == EffectType::DepthBasedOutline)
	{
		newEffect->effect = std::make_unique<DepthBasedOutline>();
		newEffect->effect->Initialize(dxCommon_, srvManager_);

		newEffect->effect->SetCameraManager(cameraManager_);
	}
	else if (type == EffectType::RadialBlur)
	{
		newEffect->effect = std::make_unique<RadialBlur>();
		newEffect->effect->Initialize(dxCommon_, srvManager_);
	}
	else if (type == EffectType::Dissolve)
	{
		newEffect->effect = std::make_unique<Dissolve>();
		newEffect->effect->Initialize(dxCommon_, srvManager_);
	}
	else if (type == EffectType::Random)
	{
		newEffect->effect = std::make_unique<Random>();
		newEffect->effect->Initialize(dxCommon_, srvManager_);
	}
	else if (type == EffectType::LinearFog)
	{
		newEffect->effect = std::make_unique<LinearFog>();
		newEffect->effect->Initialize(dxCommon_, srvManager_);

		newEffect->effect->SetCameraManager(cameraManager_);
	}
	else if (type == EffectType::MotionBlur)
	{
		newEffect->effect = std::make_unique<MotionBlur>();
		newEffect->effect->Initialize(dxCommon_, srvManager_);

		newEffect->effect->SetCameraManager(cameraManager_);
	}
	else if (type == EffectType::ColorSpace)
	{
		newEffect->effect = std::make_unique<ColorSpace>();
		newEffect->effect->Initialize(dxCommon_, srvManager_);
	}
	else
	{
		assert(0);
	}

	newEffect->type = type;

	activeEffects_[name] = std::move(newEffect);
}

void PostEffect::ResetActiveEffect()
{
	for (auto it = activeEffects_.begin(); it != activeEffects_.end(); ++it)
	{
		(*it).second->effect->Reset();
	}

	activeEffects_.clear();
}

void PostEffect::DrawImGui()
{
	// 追加したいエフェクトの名前入力用
	static char effectNameBuffer[128] = "NewEffect";

	// 選択中のエフェクトタイプ
	static int selectedEffect = 0;

	const char* effectTypeNames[] = {
		"Grayscale", "Vignette", "BoxFilter", "GaussianFilter",
		"LuminanceBasedOutline", "DepthBasedOutline", "RadialBlur",
		"Dissolve", "Random", "LinearFog", "MotionBlur", "ColorSpace"
	};

	// UI: 新しいエフェクトの追加
	ImGui::InputText("Effect Name", effectNameBuffer, IM_ARRAYSIZE(effectNameBuffer));
	ImGui::Combo("Effect Type", &selectedEffect, effectTypeNames, static_cast<int>(EffectType::EffectCount));

	if (ImGui::Button("Add Effect")) {
		std::string name = effectNameBuffer;
		if (!name.empty() && activeEffects_.find(name) == activeEffects_.end()) {
			ApplyEffect(name, static_cast<EffectType>(selectedEffect));
		}
	}

	ImGui::Separator();

	// UI: 現在のエフェクト一覧
	ImGui::Text("Active Effects:");
	for (auto it = activeEffects_.begin(); it != activeEffects_.end();) {
		ImGui::PushID(it->first.c_str());
		ImGui::Text("%s", it->first.c_str());
		ImGui::SameLine();
		if (ImGui::Button("Remove")) {
			it->second.get()->effect->Reset();
			it = activeEffects_.erase(it);  // 削除して次の要素へ
			ImGui::PopID();
			continue;
		}
		ImGui::PopID();
		++it;
	}
}
