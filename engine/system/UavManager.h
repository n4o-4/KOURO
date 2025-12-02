#pragma once
#include <cstdint>

#include "DirectXCommon.h"

// \brief UavManager
// DirectX12におけるUAV（Unordered Access View）の管理を行うクラス。
// 最大512個のUAVを管理可能で、StructuredBuffer用UAVの作成やCPU/GPUデスクリプタハンドル取得が可能。
// 描画前の準備処理やUAV割り当ても提供し、UAVの使用上限チェックも実装。

class UavManager
{
public:

	// 最大UAV数(テクスチャ枚数)
	static const uint32_t kMaxUAVCount = 512;

public: // 公開メンバ関数

	/**
	* \brief  Initialize 初期化
	* \param  dxCommon DirectXCommonのポインタ
	*/
	void Initialize(DirectXCommon* dxCommon);

	/**
	* \brief  Allocate UAV用デスクリプタを1つ割り当てる
	*/
	uint32_t Allocate();

	/**
	* \brief  PreDraw 描画前処理
	*/
	void PreDraw();

	/**
	* /brief  CreateUAVforStructuredBuffer StructuredBuffer用のUAVを作成する
	* \param  uavIndex UAVインデックス
	* \param  pResource リソースポインタ
	* \param  numElements 要素数
	* \param  structureByteStride 構造体のバイトサイズ
	*/
	void CreateUAVforStructuredBuffer(uint32_t uavIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	/**
	* \brief  GetCPUDescriptorHandle CPUデスクリプタハンドルを取得する
	* \param  index UAVインデックス
	*/
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);

	/**
	* \brief  GetGPUDescriptorHandle GPUデスクリプタハンドルを取得する
	* \param  index UAVインデックス
	*/
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

private: // 非公開メンバ関数

	/**
	* \brief  CheckUavCount UAVの数が上限に達しているかどうか確認する
	*/
	bool CheckUavCount();

private: // 非公開メンバ関数

	// DirectXCommonのポインタ
	DirectXCommon* dxCommon_ = nullptr;

	// UAV用のデスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_ = nullptr;

	// UAV用のデスクリプタサイズ
	uint32_t descriptorSize_ = 0;

	// 現在使用しているUAVインデックス
	uint32_t useIndex_ = 0;


};

