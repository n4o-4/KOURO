#pragma once
#include <cassert>
#include <dxcapi.h>
#include <dxgi1_6.h>
#include "Logger.h"
#include "D3D12Context.h"

class ShaderCompiler
{
public:

	/**
	* \brief  初期化
	*/
	void Initialize();

	/**
	* \brief  シェーダーをコンパイル
	* \param  filePath : Compilerに使用するProfile
	* \param  profile  : CompilerするShaderファイルへのパス
	*/
	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile);

private:

	/// \brief DXCユーティリティーの作成
	void CreateDXCCompiler();

private:

	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils = nullptr;

	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler = nullptr;

	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler = nullptr;

};

