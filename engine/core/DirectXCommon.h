#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <wrl.h>

#include "Logger.h"
#include "StringUtility.h"
#include "WinApp.h"
#include <dxcapi.h>
#include <format>
#include <chrono>
#include <thread>
#include <memory>
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"
#include <dxgidebug.h>

#include "Structs.h"
#include "OffScreenRendring.h"
#include "D3D12Context.h"

// \brief DirectXCommon DirectX12の初期化と描画処理をまとめて管理するクラス。  デバイスやスワップチェーンなどの生成・制御を一括で行う。

namespace Kouro
{
	class DirectXCommon
	{
	public:
		// シングルトンインスタンスの取得
		static DirectXCommon* GetInstance();

		/**
		* \brief  初期化
		* \param  winApp ウィンドウアプリケーションクラスのポインタ
		*/
		void Initialize(WinApp* winApp);

		/// \brief 終了処理
		void Finalize();

		/// \brief レンダーテクスチャ描画前処理
		void RenderTexturePreDraw();

		/// \brief レンダーテクスチャ描画後処理
		void RenderTexturePostDraw();

		/// \brief 描画前処理
		void PreDraw();

		/// \brief 描画後処理
		void PostDraw();

		/**
		* \brief  デバイス取得
		* \return デバイス
		*/
		Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return device; }

		/**
		* \brief  スワップチェーン設定の取得
		* \return スワップチェーン設定
		*/
		DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc() { return swapChainDesc; }

		/**
		* \brief  コマンドキュー取得
		* \return コマンドキュー
		*/
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue() { return commandQueue; }

		/**
		* \brief  コマンドアロケータ取得
		* \return コマンドアロケータ
		*/
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetCommandAllocator() { return commandAllocator; }

		/**
		* \brief  コマンドリスト取得
		* \return コマンドリスト
		*/
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return commandList; }

		/**
		* \brief  バックバッファ数取得
		* \return バックバッファ数
		*/
		size_t GetBackBufferCount() const { return backBufferCount_; }

		/**
		* \brief  デスクリプタヒープ作成
		* \param  heapType ヒープタイプ
		* \param  numDescriptors デスクリプタ数
		* \param  shaderVisible シェーダーから見えるかどうか
		* \return デスクリプタヒープ
		*/
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

		/**
		* \brief  深度ステンシルテクスチャリソース作成
		* \param  device デバイス
		* \param  width 幅
		* \param  height 高さ
		* \return 深度ステンシルテクスチャリソース
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, int32_t width, int32_t height);

		/**
		* \brief  シェーダーコンパイル
		* \param  filePath ファイルパス
		* \param  profile プロファイル
		* \return シェーダーブロブ
		*/
		Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile);

		/**
		* \brief  バッファリソース作成
		* \param  sizeInBytes バッファサイズ(バイト単位)
		* \return バッファリソース
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);

		/**
		* \brief  テクスチャリソース作成
		* \param  metadata テクスチャメタデータ
		* \return テクスチャリソース
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);

		/**
		* \brief  テクスチャデータアップロード
		* \param  texture テクスチャリソース
		* \param  mipImages ミップイメージ群
		* \return アップロード後のテクスチャリソース
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);

		/**
		* \brief  テクスチャ読み込み
		* \param  filePath ファイルパス
		* \return テクスチャデータ
		*/
		DirectX::ScratchImage LoadTexture(const std::string& filePath);

		/**
		* \brief  コンピュートバッファリソース作成
		* \param  sizeInBytes バッファサイズ(バイト単位)
		* \return コンピュートバッファリソース
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateComputeBufferResource(size_t sizeInBytes);

		/**
		* \brief  CPUデスクリプタハンドル取得
		* \param  descriptorHeap デスクリプタヒープ
		* \param  descriptorSize デスクリプタサイズ
		* \param  index インデックス
		* \return CPUデスクリプタハンドル
		*/
		static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, const  uint32_t descriptorSize, const  uint32_t index)
		{
			D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
			handleCPU.ptr += (descriptorSize * index);
			return handleCPU;
		}

		/**
		* \brief  GPUデスクリプタハンドル取得
		* \param  descriptorHeap デスクリプタヒープ
		* \param  descriptorSize デスクリプタサイズ
		* \param  index インデックス
		* \return GPUデスクリプタハンドル
		*/
		static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, const  uint32_t descriptorSize, const  uint32_t index) {
			D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart();
			handleGPU.ptr += (descriptorSize * index);
			return handleGPU;
		}

		/**
		* \brief  レンダーターゲットビューのCPUハンドル取得
		* \param  index インデックス
		* \return レンダーターゲットビューのCPUハンドル
		*/
		D3D12_CPU_DESCRIPTOR_HANDLE* GetRTVHandle(const int index) { return &rtvHandles[index]; }

		/**
		* \brief  レンダーターゲットビューのCPUハンドル設定
		* \param  index インデックス
		*/
		void SetRTVHandle(const int index) { rtvHandles[index].ptr = rtvHandles[index - 1].ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV); }

		/**
		* \brief  レンダーターゲットビューの設定取得
		* \return レンダーターゲットビューの設定
		*/
		D3D12_RENDER_TARGET_VIEW_DESC* GetRtvDesc() { return &rtvDesc; }

		/**
		* \brief  バックバッファのインデックス取得
		* \return バックバッファのインデックス
		*/
		UINT GetBufferIndex() { UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex(); return backBufferIndex; }

		/**
		* \brief  深度ステンシルビューのデスクリプタヒープ取得
		* \return 深度ステンシルビューのデスクリプタヒープ
		*/
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDSVHeap() { return dsvDescriptorHeap; }

		/**
		* \brief  ビューポート取得
		* \return ビューポート
		*/
		D3D12_VIEWPORT* GetViewPort() { return &viewport; }

		/**
		* \brief  シザリング矩形取得
		* \return シザリング矩形
		*/
		D3D12_RECT* GetRect() { return &scissorRect; }

		/**
		* \brief  レンダーテクスチャリソース取得
		* \return レンダーテクスチャリソース
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource>* GetRenderTextureResources() { return renderTextureResources; }

		/**
		* \brief  深度ステンシルビューのCPUハンドル取得
		* \return 深度ステンシルビューのCPUハンドル
		*/
		D3D12_CPU_DESCRIPTOR_HANDLE GetDsvHandle() { return dsvHandle; }

		/**
		* \brief  スワップチェーン取得
		* \return スワップチェーン
		*/
		IDXGISwapChain4* GetSwapChain() { return swapChain.Get(); }

		/**
		* \brief  スワップチェーンリソース取得
		* \return スワップチェーンリソース
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource>* GetSwapChainResources() { return swapChainResources; }

		/**
		* \brief  フェンス値取得
		* \return フェンス値
		*/
		uint32_t GetRenderResourceIndex() { return renderResourceIndex_; }

		/**
		* \brief  レンダーターゲットのインデックス取得
		* \return レンダーターゲットのインデックス
		*/
		uint32_t GetRenderTargetIndex() { return renderTargetIndex_; }

		/**
		* \brief  フェンス値設定
		* \param  renderResourceIndex フェンス値
		*/
		void SetRenderResourceIndex(uint32_t renderResourceIndex) { renderResourceIndex_ = renderResourceIndex; }

		/**
		* \brief  レンダーターゲットのインデックス設定
		* \param  renderTargetIndex レンダーターゲットのインデックス
		*/
		void SetRenderTargetIndex(uint32_t renderTargetIndex) { renderTargetIndex_ = renderTargetIndex; }

		/**
		* \brief  深度SRVインデックス設定
		* \param  srvIndex 深度SRVインデックス
		*/
		void SetDepthSrvIndex(uint32_t srvIndex) { depthSrvIndex_ = srvIndex; }

		/**
		* \brief  深度SRVハンドル設定
		* \param  depthHandle 深度SRVハンドル
		*/
		void SetDepthHandle(D3D12_CPU_DESCRIPTOR_HANDLE depthHandle) { depthHandle_ = depthHandle; }

		/**
		* \brief  深度ステンシルビューのCPUハンドル取得
		* \return 深度ステンシルビューのCPUハンドル
		*/
		D3D12_CPU_DESCRIPTOR_HANDLE GetDsvHandles() { return dsvHandle; }

		/**
		* \brief  深度SRVインデックス取得
		* \return 深度SRVインデックス
		*/
		uint32_t GetDepthSrvIndex() { return depthSrvIndex_; }

		/**
		* \brief  深度ステンシルリソース取得
		* \return 深度ステンシルリソース
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource> GetDepthStencilResource() { return depthStencilResource; }

		/**
		* \brief  深度SRVハンドル取得
		* \return 深度SRVハンドル
		*/
		D3D12_CPU_DESCRIPTOR_HANDLE GetDepthSrvHandle() { return depthHandle_; }

		/**
		* \brief  D3D12コンテキスト作成
		* \return D3D12コンテキスト
		*/
		D3D12Context CreateD3D12Context() const;

	private:

		/// \brief デバイス初期化
		void InitializeDevice();

		/// \brief コマンド関連初期化
		void InitializeCommands();

		/// \brief スワップチェーン作成
		void CreateSwapChain();

		/// \brief 深度バッファ作成
		void CreateDepthBuffer();

		/// \brief デスクリプタヒープ作成
		void CreateDescriptorHeaps();

		/// \brief レンダーターゲットビュー作成	
		void InitializeRenderTergetView();

		/// \brief 深度ステンシルビュー作成
		void InitializeDepthStencilView();

		/// \brief フェンス初期化
		void InitializeFence();

		/// \brief ビューポート初期化
		void InitializeViewPort();

		/// \brief シザリング矩形初期化
		void Scissor();

		/// \brief DXCユーティリティ作成
		void CreateDXCCompiler();

		/// \brief ImGui初期化
		void InitializeImGui();

		/**
		* \brief  レンダーテクスチャリソース作成
		* \param  device デバイス
		* \param  width 幅
		* \param  height 高さ
		* \param  format フォーマット
		* \param  clearColor クリアカラー
		* \param  resourceStates リソースステート
		* \return レンダーテクスチャリソース
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor, D3D12_RESOURCE_STATES resourceStates);

		/// \brief レンダーテクスチャ用RTV作成
		void CreateRenderTextureRTV();

		/// \brief FPS固定初期化
		void InitializeFixFPS();

		/// \brief FPS固定更新
		void UpdateFixFPS();

		/// \brief オフスクリーン用ルートシグネチャ作成
		void CreateOffScreenRootSignature();

		/// \brief オフスクリーン用パイプライン作成
		void CreateOffScreenPipeLine();

	private:

		/*---------------------
		* メンバー変数
		---------------------*/

		static std::unique_ptr<DirectXCommon> instance;

		friend std::unique_ptr<DirectXCommon> std::make_unique<DirectXCommon>();
		friend std::default_delete<DirectXCommon>;

		DirectXCommon() = default;
		~DirectXCommon() = default;
		DirectXCommon(DirectXCommon&) = delete;
		DirectXCommon& operator=(DirectXCommon&) = delete;

		// WindowAPI
		WinApp* winApp = nullptr;

		// DirectX12デバイス
		Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;

		// DXGIファクトリ
		Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory = nullptr;

		Microsoft::WRL::ComPtr<IDXGISwapChain4>  swapChain = nullptr;

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

		Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource = nullptr;

		Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResources[2] = { nullptr };

		Microsoft::WRL::ComPtr< ID3D12Fence> fence = nullptr;

		D3D12_VIEWPORT viewport{};

		D3D12_RECT scissorRect{};

		Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils = nullptr;

		Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler = nullptr;

		Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler = nullptr;

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[4] = {};

		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = {};

		//D3D12_RESOURCE_BARRIER barrier{};

		HANDLE fenceEvent = nullptr;

		uint32_t fenceValue = 0;

		uint32_t backBufferCount_ = 2;

		// 記録時間(FPS固定用)
		std::chrono::steady_clock::time_point reference_;

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue = nullptr;

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator = nullptr;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList = nullptr;

		//uint32_t descriptorSizeSRV = 0;

		uint32_t descriptorSizeRTV = 0;

		uint32_t descriptorSizeDSV = 0;

		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap = nullptr;

		//Microsoft::WRL::ComPtr< ID3D12DescriptorHeap> srvDescriptorHeap = nullptr;

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap = nullptr;

		Microsoft::WRL::ComPtr<ID3D12Debug1> debugController = nullptr;

		Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;


		Microsoft::WRL::ComPtr< ID3DBlob> signatureBlob = nullptr;

		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

		//========================================================
		// 深度リソース用
		uint32_t depthSrvIndex_ = 0;

		D3D12_CPU_DESCRIPTOR_HANDLE depthHandle_ = {};

		//========================================================
		// ポストエフェクト用

		Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResources[2] = { nullptr };

		uint32_t renderResourceIndex_ = 0;
		uint32_t renderTargetIndex_ = 1;
	};
}