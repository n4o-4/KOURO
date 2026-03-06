#include "ShaderManager.h"
#include "Logger.h"

namespace Kouro
{
	void ShaderManager::AddShader(const std::string& key, const Shader::GraphicsShader& shader)
	{
		// キーが既に存在するか確認
		if (shaderCache_.contains(key))
		{
			// キーが存在する場合は警告を出す
			Logger::Log("Shader with key \"" + key + "\" already exists. Overwriting.");
		}

		// シェーダーをキャッシュに追加（存在する場合は上書き）
		shaderCache_[key] = shader;
	}

	Shader::GraphicsShader Kouro::ShaderManager::GetShader(const std::string& key) const
	{
		// キーが存在するか確認
		auto it = shaderCache_.find(key);

		// キーが存在する場合はシェーダーを返す
		if (it != shaderCache_.end())
		{
			return it->second;
		}

		// キーが存在しない場合は警告を出して空のシェーダーを返す
		else
		{
			Logger::Log("Shader with key \"" + key + "\" not found. Returning an empty shader.");

			return Shader::GraphicsShader{};
		}
	}
}
