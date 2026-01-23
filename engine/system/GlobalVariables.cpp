#include "GlobalVariables.h"
#include <fstream>
#include <iostream>

namespace Kouro
{
    void GlobalVariables::SaveFile(const std::string& item, uint32_t value)
    {
        json root;

        // 既存のファイルを読み込む（存在しない場合は新規作成）
        std::ifstream inFile("Resources/Data/global.json");
        if (inFile.is_open())
        {
            inFile >> root;
            inFile.close();
        }
        else
        {
            root = json::object();
        }

        // 値を設定
        root[item] = value;

        // ファイルに保存（整形出力）
        std::ofstream outFile("Resources/global.json");
        if (outFile.is_open())
        {
            outFile << std::setw(4) << root << std::endl;
            outFile.close();
        }
    }

    uint32_t GlobalVariables::LoadFile(const std::string& item, uint32_t defaultValue)
    {
        json root;

        // ファイル読み込み
        std::ifstream inFile("Resources/global.json");
        if (inFile.is_open())
        {
            try {
                inFile >> root;
                inFile.close();
            }
            catch (...) {
                std::cerr << "Error: JSONファイルの読み込みに失敗しました。\n";
                return defaultValue;
            }
        }
        else
        {
            std::cerr << "Warning: global.json が見つかりません。\n";
            return defaultValue;
        }

        // 指定キーが存在するか確認
        if (root.contains(item))
        {
            try {
                return root[item].get<uint32_t>();
            }
            catch (...) {
                std::cerr << "Error: キー[" << item << "]の型がuint32_tではありません。\n";
                return defaultValue;
            }
        }
        else
        {
            std::cerr << "Warning: キー[" << item << "]が存在しません。\n";
            return defaultValue;
        }
    }
}