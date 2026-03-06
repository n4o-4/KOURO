#pragma once
#include <optional>
#include <wrl.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

/// \brief  シェーダー関連の構造体を定義するヘッダーファイル
namespace Kouro::Shader
{
	/**
	* \brief  グラフィックスシェーダーを表す構造体
	* \param  vertexShader : 頂点シェーダーのバイナリデータ
	* \param  pixelShader  : ピクセルシェーダーのバイナリデータ
	*/
	struct GraphicsShader
	{
		Microsoft::WRL::ComPtr<IDxcBlob> vertexShader = nullptr;
		Microsoft::WRL::ComPtr<IDxcBlob> pixelShader = nullptr;
	};
}