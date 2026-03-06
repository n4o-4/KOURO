#pragma once
#include <cassert>
#include "ShaderTypes.h"
#include "Logger.h"
#include "D3D12Context.h"

namespace Kouro
{
	/// \brief シェーダーコンパイラークラス
	class ShaderCompiler
	{
	public

		enum class ShaderType
		{
			VertexShader,
			PixelShader
		};

	public:

		/// \brief 初期化
		void Initialize();

		/**
		* \brief  シェーダーのコンパイル
		* \param  filePath コンパイルするシェーダーファイルのパス
		* \param  profile シェーダープロファイル（例: L"vs_6_0"、L"ps_6_0"）
		*/
		Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile);

		/**
		* \brief  シェーダープロファイルの取得
		* \param  type シェーダーの種類（頂点シェーダー、ピクセルシェーダーなど）
		* \return シェーダープロファイルの文字列（例: L"vs_6_0"、L"ps_6_0"）
		*/
		const std::wstring& GetPixelShaderProfile(ShaderType type) const;

	private:

		/// \brief DXCコンパイラー関連のインターフェースを生成、初期化する
		void CreateDXCCompiler();

	private:

		/// DXCコンパイラー関連のインターフェース
		Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils = nullptr; //!< DXCユーティリティーインターフェース
		Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler = nullptr; //!< DXCコンパイラーインターフェース
		Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler = nullptr; //!< DXCインクルードハンドラーインターフェース

		/// シェーダープロファイル
		const std::wstring pixelShaderProfile_ = L"ps_6_0"; //!< ピクセルシェーダープロファイル
		const std::wstring vertexShaderProfile_ = L"vs_6_0"; //!< バーテックスシェーダープロファイル
	};
}