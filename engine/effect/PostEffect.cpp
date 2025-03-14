#include "PostEffect.h"

void PostEffect::Initialize(DirectXCommon* dxCommon, SrvManager* srvManager)
{
	dxCommon_ = dxCommon;

	srvManager_ = srvManager;
}

void PostEffect::Update()
{
	for (auto it = activeEffects_.begin(); it != activeEffects_.end(); ++it) {


		(*it).get()->effect->Update();


	}
}

void PostEffect::Draw()
{
    // 書き込む方のインデックス
	uint32_t targetIndex = dxCommon_->GetRenderTargetIndex();
	// 読み込む方のインデックス
	uint32_t resourceIndex = dxCommon_->GetRenderResourceIndex();

	for (auto it = activeEffects_.begin(); it != activeEffects_.end(); ++it) {

		// 
		(*it).get()->effect->Draw(targetIndex,resourceIndex);

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

	//for (int i = 0; i < static_cast<int>(EffectType::EffectCount); ++i)
	//{
	//	if (!effect.isActive[i])
	//	{
	//		continue;
	//	}

	//	renderTextureIndex = 2 + targetIndex;
	//	auto it = std::next(effect.pipelines_.begin(), i);

	//	// 描画先のRTVを設定する
	//	dxCommon_->GetCommandList()->OMSetRenderTargets(1, &*dxCommon_->GetRTVHandle(renderTextureIndex), false, nullptr);

	//	dxCommon_->GetCommandList()->SetGraphicsRootSignature(it->get()->rootSignature.Get());
	//	dxCommon_->GetCommandList()->SetPipelineState(it->get()->pipelineState.Get());

	//	// Random
	//	if (i == 9)
	//	{
	//		dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(0, randomResource_.Get()->GetGPUVirtualAddress());

	//		dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	//		// 次のエフェクトがアクティブか確認
	//		int activeCount = 0;
	//		for (int sub = i + 1; sub < static_cast<int>(EffectType::EffectCount); ++sub)
	//		{
	//			if (effect.isActive[sub])
	//			{
	//				++activeCount;
	//			}
	//		}

	//		// 現在書き込んだ方を読み込み用に変換
	//		D3D12_RESOURCE_BARRIER barrier1{};
	//		barrier1.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//		barrier1.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//		barrier1.Transition.pResource = dxCommon_->GetRenderTextureResources()[targetIndex].Get();
	//		barrier1.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//		barrier1.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//		dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier1);

	//		// 今読み込んだ方を描画用に変換
	//		D3D12_RESOURCE_BARRIER barrier2{};
	//		barrier2.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//		barrier2.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//		barrier2.Transition.pResource = dxCommon_->GetRenderTextureResources()[resourceIndex].Get();
	//		barrier2.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//		barrier2.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	//		dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier2);

	//		std::swap(targetIndex, resourceIndex);

	//		dxCommon_->SetRenderTargetIndex(targetIndex);
	//		dxCommon_->SetRenderResourceIndex(resourceIndex);

	//		break;

	//	}

	//	// 中間テクスチャの切り替え
	//	D3D12_GPU_DESCRIPTOR_HANDLE srvHandle;
	//	if (resourceIndex == 0)
	//	{
	//		srvHandle = TextureManager::GetInstance()->GetSrvHandleGPU("RenderTexture0");
	//	}
	//	else
	//	{
	//		srvHandle = TextureManager::GetInstance()->GetSrvHandleGPU("RenderTexture1");
	//	}

	//	assert(srvHandle.ptr != 0);

	//	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(0, srvHandle);

	//	// 深度検出でのアウトライン || LinearFog
	//	if (i == 6 || i == 10)
	//	{
	//		// 現在書き込んだ方を読み込み用に変換
	//		D3D12_RESOURCE_BARRIER depthbarrier{};
	//		depthbarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//		depthbarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//		depthbarrier.Transition.pResource = dxCommon_->GetDepthStencilResource().Get();

	//		if (depthbarrier.Transition.pResource == nullptr)
	//		{
	//			assert(0);
	//		}

	//		depthbarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	//		depthbarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//		dxCommon_->GetCommandList()->ResourceBarrier(1, &depthbarrier);
	//		// 指定した色で画面全体をクリアする
	//		//float clearColor[] = { 1.0f,0.0f,0.0f,1.0f }; // 青っぽい色 RGBAの順
	//		//dxCommon_->GetCommandList()->ClearRenderTargetView(*DirectXCommon::GetInstance()->GetRTVHandle(renderTextureIndex), clearColor, 0, nullptr);


	//		// ディスクリプタテーブルを設定 (深度テクスチャ SRV)
	//		dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(dxCommon_->GetDepthSrvIndex()));

	//		// 定数バッファを設定
	//		dxCommon_->GetCommandList()->SetGraphicsRootConstantBufferView(2, depthOutlineResource_.Get()->GetGPUVirtualAddress());

	//		dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	//		// 次のエフェクトがアクティブか確認
	//		int activeCount = 0;
	//		for (int sub = i + 1; sub < static_cast<int>(EffectType::EffectCount); ++sub)
	//		{
	//			if (effect.isActive[sub])
	//			{
	//				++activeCount;
	//			}
	//		}

	//		// 現在書き込んだ方を読み込み用に変換
	//		D3D12_RESOURCE_BARRIER barrier1{};
	//		barrier1.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//		barrier1.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//		barrier1.Transition.pResource = dxCommon_->GetRenderTextureResources()[targetIndex].Get();
	//		barrier1.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//		barrier1.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//		dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier1);

	//		// 今読み込んだ方を描画用に変換
	//		D3D12_RESOURCE_BARRIER barrier2{};
	//		barrier2.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//		barrier2.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//		barrier2.Transition.pResource = dxCommon_->GetRenderTextureResources()[resourceIndex].Get();
	//		barrier2.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//		barrier2.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	//		dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier2);

	//		std::swap(targetIndex, resourceIndex);

	//		dxCommon_->SetRenderTargetIndex(targetIndex);
	//		dxCommon_->SetRenderResourceIndex(resourceIndex);

	//		depthbarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//		depthbarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;

	//		dxCommon_->GetCommandList()->ResourceBarrier(1, &depthbarrier);

	//		break;

	//	}

	//	// Dissolve
	//	if (i == 8)
	//	{
	//		// 指定した色で画面全体をクリアする
	//		float clearColor[] = { 1.0f,0.0f,0.0f,1.0f }; // 青っぽい色 RGBAの順
	//		DirectXCommon::GetInstance()->GetCommandList()->ClearRenderTargetView(*DirectXCommon::GetInstance()->GetRTVHandle(renderTextureIndex), clearColor, 0, nullptr);

	//	    dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(1, TextureManager::GetInstance()->GetSrvHandleGPU("Resources/noise0.png"));

	//		dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	//		// 次のエフェクトがアクティブか確認
	//		int activeCount = 0;
	//		for (int sub = i + 1; sub < static_cast<int>(EffectType::EffectCount); ++sub)
	//		{
	//			if (effect.isActive[sub])
	//			{
	//				++activeCount;
	//			}
	//		}

	//		// 現在書き込んだ方を読み込み用に変換
	//		D3D12_RESOURCE_BARRIER barrier1{};
	//		barrier1.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//		barrier1.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//		barrier1.Transition.pResource = dxCommon_->GetRenderTextureResources()[targetIndex].Get();
	//		barrier1.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//		barrier1.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//		dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier1);

	//		// 今読み込んだ方を描画用に変換
	//		D3D12_RESOURCE_BARRIER barrier2{};
	//		barrier2.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//		barrier2.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//		barrier2.Transition.pResource = dxCommon_->GetRenderTextureResources()[resourceIndex].Get();
	//		barrier2.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//		barrier2.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	//		dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier2);

	//		std::swap(targetIndex, resourceIndex);

	//		dxCommon_->SetRenderTargetIndex(targetIndex);
	//		dxCommon_->SetRenderResourceIndex(resourceIndex);

	//		break;

	//	}

	//	dxCommon_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	//	// 次のエフェクトがアクティブか確認
	//	int activeCount = 0;
	//	for (int sub = i + 1; sub < static_cast<int>(EffectType::EffectCount); ++sub)
	//	{
	//		if (effect.isActive[sub])
	//		{
	//			++activeCount;
	//		}
	//	}

	//	if (activeCount == 0)
	//	{
	//		// 現在書き込んだ方を読み込み用に変換
	//		D3D12_RESOURCE_BARRIER barrier1{};
	//		barrier1.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//		barrier1.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//		barrier1.Transition.pResource = dxCommon_->GetRenderTextureResources()[targetIndex].Get();
	//		barrier1.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//		barrier1.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//		dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier1);

	//		// 今読み込んだ方を描画用に変換
	//		D3D12_RESOURCE_BARRIER barrier2{};
	//		barrier2.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//		barrier2.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//		barrier2.Transition.pResource = dxCommon_->GetRenderTextureResources()[resourceIndex].Get();
	//		barrier2.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//		barrier2.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	//		dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier2);

	//		std::swap(targetIndex, resourceIndex);

	//		dxCommon_->SetRenderTargetIndex(targetIndex);
	//		dxCommon_->SetRenderResourceIndex(resourceIndex);

	//		break;
	//	}

	//	// 現在書き込んだ方を読み込み用に変換
	//	D3D12_RESOURCE_BARRIER barrier{};
	//	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//	barrier.Transition.pResource = dxCommon_->GetRenderTextureResources()[targetIndex].Get();
	//	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);

	//	// 今読み込んだ方を描画用に変換
	//	D3D12_RESOURCE_BARRIER subBarrier{};
	//	subBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//	subBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//	subBarrier.Transition.pResource = dxCommon_->GetRenderTextureResources()[resourceIndex].Get();
	//	subBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//	subBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	//	dxCommon_->GetCommandList()->ResourceBarrier(1, &subBarrier);

	//	std::swap(targetIndex, resourceIndex);
	//}


}

void PostEffect::ApplyEffect(EffectType type)
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
	}

	newEffect->type = type;

	activeEffects_.push_back(std::move(newEffect));
}