#pragma once
#include <string>
#include <wrl.h>

#include <d3d12.h>
//#include <dxgi1_6.h>
#include "DirectXCommon.h"
#include "SrvManager.h"

#include "externals/DirectXTex/DirectXTex.h"
#include <unordered_map>
#include "externals//DirectXTex/d3dx12.h"
#include <vector>
// テクスチャマネージャー
class TextureManager
{
public:
	// シングルトンインスタンスの取得
	static TextureManager* GetInstance();

	// 初期化
	void Initialize(DirectXCommon* dxCommon,SrvManager* srvManager);

	// 終了
	void Finalize();

	// Textureデータを読み込む関数
	void LoadTexture(const std::string& filePath);

	// SRVインデックスの開始番号
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);

	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(std::string filePath);

	// メタデータを取得
	const DirectX::TexMetadata& GetMetaData(std::string filePath);

	void CreateRenderTextureMetaData();

private:
	static std::unique_ptr<TextureManager> instance;

	friend std::unique_ptr<TextureManager> std::make_unique<TextureManager>();
	friend std::default_delete<TextureManager>;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;

private:
	// テクスチャー1枚分のデータ
	struct TextureData {
		DirectX::TexMetadata metadata;
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		uint32_t srvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

	// テクスチャデータ
	std::unordered_map<std::string, TextureData> textureDatas;

	DirectXCommon* dxCommon_;

	SrvManager* srvManager_;

	// SRVインデックスの開始番号
	static uint32_t kSRVIndexTop;

	Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;

	HANDLE fenceEvent = nullptr;

	uint32_t fenceValue;
};

