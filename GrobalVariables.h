#pragma once
#include <string>
#include <json.hpp>

using json = nlohmann::json;

// \brief GrobalVariables
// ゲームのグローバル変数や設定値の保存・読み込みを管理するクラス。
// SaveFile関数で指定したアイテム名と値を保存し、LoadFile関数で保存済みの値を取得で

class GrobalVariables
{
public:

	void SaveFile(const std::string& item,uint32_t value);

	uint32_t LoadFile(const std::string& item, uint32_t defaultValue = 0);

};

