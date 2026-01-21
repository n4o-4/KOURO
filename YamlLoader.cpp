#include "YamlLoader.h"
#include <filesystem>
#include <iostream>

YAML::Node KOURO::YamlLoader::LoadYamlFile(const std::string& fileName)
{
	// 例外処理を使用してYAMLファイルの読み込み
    try{
        // YAMLファイルのフルパスを作成
        std::filesystem::path fullPath = std::filesystem::path(rootPath_) / fileName;

		// ファイルの存在確認
        if (std::filesystem::exists(fullPath)) {
            std::cout << "ファイルが存在します！" << std::endl;
        }
        else {
            std::cout << "ファイルが存在しません..." << std::endl;
        }

		// YAMLファイルを読み込む
        YAML::Node config = YAML::LoadFile(fullPath.string());

		// ノードが未定義の場合のエラーメッセージ
        if (!config.IsDefined())
        {
            std::cerr << "YAMLロード失敗: Nodeが未定義です！" << std::endl;
        }

		// 読み込んだYAMLノードを返す
        return config;
    }

	// 例外が発生した場合の処理
    catch (const std::exception& e)
    {
		// エラーメッセージを表示
        std::cerr << "YAML Load Error: " << e.what() << std::endl;
        
        // 空のノードを返す
        return YAML::Node();
    }
}
