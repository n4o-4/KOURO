#include "UavManager.h"

namespace Kouro
{
	void UavManager::Initialize(DirectXCommon* dxCommon)
	{
		// メンバ変数に記録
		dxCommon_ = dxCommon;

		// デスクリプタヒープの生成
		descriptorHeap_ = dxCommon->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxUAVCount, true);

		// デスクリプタ1個分のサイズを取得して記録
		descriptorSize_ = dxCommon_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	uint32_t UavManager::Allocate()
	{
		// return する番号を一旦記録しておく
		int index = useIndex_;

		// 次回のために番号を1進める
		useIndex_++;

		// 記録した番号をreturn
		return index;
	}

	void UavManager::PreDraw()
	{
		// 描画用のDescriptorHeapの設定
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { descriptorHeap_ };
		dxCommon_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());
	}

	void UavManager::CreateUAVforStructuredBuffer(uint32_t uavIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride)
	{
		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;  // 構造化バッファではUnknown

		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = numElements;             // 要素数（例: 1024）
		uavDesc.Buffer.StructureByteStride = structureByteStride; // sizeof(ParticleCS)
		uavDesc.Buffer.CounterOffsetInBytes = 0;              // カウンタ付きならここに指定
		uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;    // 基本はNONE

		// デバイスを使って SRV を作成する
		dxCommon_->GetDevice()->CreateUnorderedAccessView(pResource, nullptr, &uavDesc, GetCPUDescriptorHandle(uavIndex));
	}

	D3D12_CPU_DESCRIPTOR_HANDLE UavManager::GetCPUDescriptorHandle(uint32_t index)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_->GetCPUDescriptorHandleForHeapStart();

		// インデックス分だけオフセットを進める
		handleCPU.ptr += (descriptorSize_ * index);

		return handleCPU;
	}

	D3D12_GPU_DESCRIPTOR_HANDLE UavManager::GetGPUDescriptorHandle(uint32_t index)
	{

		D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_->GetGPUDescriptorHandleForHeapStart();

		// インデックス分だけオフセットを進める
		handleGPU.ptr += (descriptorSize_ * index);

		return handleGPU;
	}

	bool UavManager::CheckUavCount()
	{
		// 使用しているUAVインデックスが上限に達しているかどうか確認する
		if (useIndex_ < kMaxUAVCount)
		{
			// 上限に達していない
			return true;
		}
		else
		{
			// 上限に達している
			return false;
		}
	}
}