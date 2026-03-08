#pragma once
// Standard Library
#include <string>
#include <unordered_map>

// Third Party

// DirectX / Windows
#include <dxcapi.h>
#include <wrl.h>

// Engine
#include "ShaderTypes.h"

// Game

namespace Kouro
{
	/// \brief  シェーダーマネージャークラス
	class ShaderManager
	{
	public: // 公開メンバ関数

		/**
		* \brief  シェーダーをマネージャーに追加する
		* \param  key : シェーダーの識別キー
		*/
		void AddShader(const std::string& key, const Shader::GraphicsShader& shader);

		/**
		* \brief  シェーダーをマネージャーから取得する
		* \param  key : シェーダーの識別キー
		* \return シェーダーの構造体（GraphicsShader）。キーが存在しない場合は空の構造体を返す
		*/
		const Shader::GraphicsShader& GetShader(const std::string& key) const;

	private: // 非公開メンバ関数

		std::unordered_map<std::string, Shader::GraphicsShader> shaderCache_; //!< シェーダーのキャッシュマップ。キーはシェーダーファイルのパス、値はコンパイルされたシェーダーの構造体

	};
}