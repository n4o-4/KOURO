#include "ShaderCompiler.h"

#include <format>

#include "StringUtils.h"

namespace Kouro
{
	void ShaderCompiler::Initialize()
	{
		// DXCコンパイラー関連のインターフェースを生成、初期化する
		CreateDXCCompiler();
	}

	Microsoft::WRL::ComPtr<IDxcBlob> ShaderCompiler::CompileShader(const std::wstring& filePath, const wchar_t* profile)
	{
		HRESULT hr;

		// hlslファイルを読み込む
		Microsoft::WRL::ComPtr<IDxcBlobEncoding> shaderSource = nullptr;
		hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);

		// 読めなかったら止める
		assert(SUCCEEDED(hr));

		// 読み込んだファイルの内容を設定する
		DxcBuffer shaderSourceBuffer;
		shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
		shaderSourceBuffer.Size = shaderSource->GetBufferSize();
		shaderSourceBuffer.Encoding = DXC_CP_UTF8; // UTFの文字コードであることを通知

		LPCWSTR arguments[] =
		{
			filePath.c_str(), // コンパイル対象のhlslファイル名
			L"-E",L"main", // エントリーポイントの指定。基本的にmain以外にはしない
			L"-T", profile, // ShaderProfileの設定
			L"-Zi", L"-Qembed_debug", // デバッグ用の情報を埋め込む
			L"-O3"/*"-Od"*/,    // 最適化しておく
			L"-Zpr",   // メモリレイアウトは最優先
		};

		// 実際にShaderをコンパイルする
		Microsoft::WRL::ComPtr<IDxcResult> shaderResult = nullptr;
		hr = dxcCompiler.Get()->Compile(
			&shaderSourceBuffer,
			arguments,
			_countof(arguments),
			includeHandler.Get(),
			IID_PPV_ARGS(&shaderResult)
		);

		// コンパイル結果が得られなかったら止める
		assert(shaderResult != nullptr);

		// コンパイルエラーでなくdxcが起動出来ないなど致命的な状況
		assert(SUCCEEDED(hr));

		// 警告・エラーが出たらログを出して止める
		Microsoft::WRL::ComPtr<IDxcBlobUtf8> shaderError = nullptr;
		shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
		if (shaderError != nullptr && shaderError->GetStringLength() != 0)
		{
			Logger::Log(shaderError->GetStringPointer());

			// 警告・エラー
			assert(false);
		}

		// コンパイル結果から実行のバイナリ部分を取得
		Microsoft::WRL::ComPtr<IDxcBlob> shaderBlob = nullptr;
		hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
		assert(SUCCEEDED(hr));

		// 成功したログを出す
		Logger::Log(StringUtility::ConvertString(std::format(L"Compile Succeeded, path:{},profile:{}\n", filePath, profile)));

		// 実行用のバイナリを返却
		return shaderBlob;
	}

	const std::wstring& ShaderCompiler::GetPixelShaderProfile(ShaderType type) const
	{
		switch (type)
		{
		case ShaderType::VertexShader:
			return vertexShaderProfile_;
		case ShaderType::PixelShader:
			return pixelShaderProfile_;
		default:
			assert(false);
			return pixelShaderProfile_;
		}
	}

	void ShaderCompiler::CreateDXCCompiler()
	{
		HRESULT hr;

		/// DXCコンパイラー関連のインターフェースを生成、初期化する

		// DXCユーティリティーインターフェースの生成
		hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
		assert(SUCCEEDED(hr));

		// DXCコンパイラーインターフェースの生成
		hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
		assert(SUCCEEDED(hr));

		// 現時点でincludeはしていないが、includeに対応するための設定をしておく
		hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
		assert(SUCCEEDED(hr));
	}
}

