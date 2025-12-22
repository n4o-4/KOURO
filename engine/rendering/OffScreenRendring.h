#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <dxgi1_6.h>
#include <cassert>
#include <dxcapi.h>
#include <format>
#include <memory>
#include "externals/DirectXTex/DirectXTex.h"
#include <dxgidebug.h>
#include "Structs.h"

class OffScreenRendring
{
public: // 公開メンバ関数

	/// \brief 初期化
	void Initialzie();

	/// \brief 描画前処理
	void PreDraw();

	/// \brief 描画後処理
	void PostDraw();

	/// \brief レンダーテクスチャリソースの取得
	void SetRenderTextureResource(ID3D12Resource* renderTextureResources) { *renderTextureResources_ = renderTextureResources; }

private: // 非公開メンバ関数

	/// \brief レンダーテクスチャリソースの作成
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor);

public: // 公開メンバ変数

private: // 非公開メンバ変数

	ID3D12Resource* renderTextureResources_[2] = {nullptr};

	D3D12_RESOURCE_BARRIER barrier{};
};