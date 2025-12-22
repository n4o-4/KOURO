#pragma once
#include "DirectXCommon.h"

// \brief SrvManager
// DirectX12におけるShader Resource View(SRV)管理クラス。  
// SRVの生成・割り当て・デスクリプタヒープ管理を行い、テクスチャや構造化バッファをGPUに提供する。

class SrvManager
{
public:

	// 最大SRV数(テクスチャ枚数)
	static const uint32_t kMaxSRVCount;

private:

    DirectXCommon* dxCommon_ = nullptr;

	// SRV用のデスクリプタサイズ
	uint32_t descriptorSize = 0;

	// SRV用デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;

	// 次に使用するSRVインデックス
	uint32_t useIndex = 0;

public:

	/**
	* \brief  SrvManagerの初期化
	* \param  dxCommon DirectXCommonのポインタ
	*/
	void Initialize(DirectXCommon* dxCommon);

	/**
	* \brief  未使用のIndexを取得する
	* \return Index
	*/
	uint32_t Allocate();

	/**
	* \brief  indexからCPU Descriptor Handleを取得する
	* \param  idnex : DescriptorHeap上のindex
	* \return CPU Descriptor Handle
	*/
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);

	/**
	* \brief  indexからGPU Descriptor Handleを取得する
	* \parma  index : DescriptorHeap上のindex
	* \return GPU Descriptor Handle
	+*/
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

	/**
    * @brief Texture2D用のShader Resource View(SRV)を作成する
    * @param srvIndex  : 作成先のDescriptorHeap上のindex
	* @param pResource : SRVを作成する対象のTextureリソース
	* @param format    : フォーマット（DXGI_FORMAT）
	* @param mipLevels : ミップマップレベル数
	*/
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels);

	
	/**
	* @brief  offScreenRendring用のTextureに対応するSRVを作成する
	* @details RenderTargetとして使用されるオフスクリーンTextureを、
	*          シェーダーでサンプリング可能にするためのShader Resource View(SRV)を生成します。
	* 
	* @param srvIndex : SRV DescripotrHeap上のindex
	* @param rtvIndex : 作成元となるRenderTextureのindex
	*/
	void CreateOffScreenTexture(uint32_t srvIndex,uint32_t rtvIndex);

	/**
	* @brief Structured Buffer用のShader Resource View(SRV)を作成する
	* @details 指定されたStructured Bufferリソースに対して、
	*          シェーダーから読み取り可能なSRVをディスクリプタヒープ上に作成します。
	*          Formatは構造化バッファのためDXGI_FORMAT_UNKNOWNを指定します。
	* 
	* @param : srvIndex SRV DescriptorHeap上のindex
	* @param : pResource SRVを作成する対象のStructured Bufferリソース
	* @param : numElements バッファ内の要素数
	* @param : structureByteStride 各要素のバイトサイズ（StructureByteStride）
	*/
	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	/**
	* @brief CubeMap用のShader Resource View(SRV)を作成します。
	* @details 指定されたCubeMapリソースに対して、
	*          シェーダーからサンプリング可能なShader Resource View(SRV)を作成する
	*          ViewDimensionはD3D12_SRV_DIMENSION_TEXTURECUBEに設定されます
	。
	* @param format    : CubeMapのフォーマット（DXGI_FORMAT）
	* @param srvIndex  : SRV DescriptorHeap上のindex
	* @param pResource : SRVを作成する対象のCubeMapリソース
	*/
	void CreateSRVforCubeMap(DXGI_FORMAT format, uint32_t srvIndex, ID3D12Resource* pResource);

	/// \brief 描画前の設定
	void PreDraw();

	// SRVセットコマンド
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);

	/**
	* \brief  srvIndexが上限に達していないか確認する
	* \return - true  : 上限に達していない場合
	* \return - false : 上限に達していた場合
	*/
	bool CheckSrvCount();

	/**
	* \brief  descriptorHeapを取得する
	* \return descriptorHeap
	*/
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return descriptorHeap; }

	/**
	* \brief  descriptorSizeを取得する
	* \return descriptorSize
	*/
	uint32_t GetDescriptorSize() { return descriptorSize; }
private:

	/// \brief 深度バッファ用のSRVを作成する
	void CreateSrvForDepth();


};

