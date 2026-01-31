#include "UIManager.h"
#include "YamlLoader.h"

void UIManager::LoadUIFromFile(const std::string& filePath)
{
	// YAMLファイルの読み込み
	YAML::Node config = KOURO::YamlLoader::LoadYamlFile(filePath);


}
