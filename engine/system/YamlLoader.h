#pragma once
#include <string>

// Externals
#include <yaml-cpp/yaml.h>

// Engine
namespace KOURO
{

	/// \brief YamlLoader YAMLローダークラス

	class YamlLoader
	{
	public:

		/**
		* \brief  LoadYamlFile YAMLファイルを読み込む
		* \param  fileName ファイル名
		* \return YAMLノード
		*/
		static YAML::Node LoadYamlFile(const std::string& fileName);

	private:

		static inline std::string rootPath_ = "Resources/config/"; //!< ルートパス

	};
}