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

// \brief TextureManager
// DirectX12用テクスチャ管理クラス。
// テクスチャの読み込み、SRV生成、GPUハンドル取得、メタデータ管理を行う。
// シングルトンとして利用され、複数のテクスチャを効率的に扱える。

namespace Kouro
{
	class TextureManager
	{
	public:
		// シングルトンインスタンスの取得
		static TextureManager* GetInstance();

		/**
		* \brief  テクスチャマネージャーの初期化
		* \param  dxCommon   DirectXCommonのポインタ
		* \param  srvManager srvManagerのポインタ
		*/
		void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager);

		/// \brief Finalize 終了
		void Finalize();

		/**
		* \brief  読み込み
		* \param  filePath : ファイルパス
		*/
		void LoadTexture(const std::string& filePath);

		/**
		* \brief  ファイルパスからインデックスを取得する
		* \param  filePath : ファイルパス
		* \return インデックス
		*/
		uint32_t GetTextureIndexByFilePath(const std::string& filePath);

		/**
		* \brief  ファイルパスからGPUハンドルを取得する
		* \param  filePath : ファイルパス
		* \reutne GPUハンドル
		*/
		D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(std::string filePath);

		/**
		* \brief  ファイルパスからメタデータを取得する
		* \param  filePath : ファイルパス
		*/
		const DirectX::TexMetadata& GetMetaData(std::string filePath);

		/// \brief オフスクリーンレンダリング用のテクスチャを作る
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

		DirectXCommon* dxCommon_ = nullptr;

		SrvManager* srvManager_ = nullptr;

		// SRVインデックスの開始番号
		static uint32_t kSRVIndexTop;

		Microsoft::WRL::ComPtr<ID3D12Fence> fence = nullptr;

		HANDLE fenceEvent = nullptr;

		uint32_t fenceValue = 0;
	};
}