#pragma once
#include <string>
#include <fstream>
#include <cassert>
#include <json.hpp>
#include <regex>

#include "Kouro.h"
#include "Structs.h"

// \brief LevelLoader  
// JSONファイルからレベルデータを読み込み、オブジェクト情報やワールド変換を管理するクラス。  
// 各オブジェクトの名前・タイプ・3Dモデル・位置情報を整理してゲーム内シーン構築に利用される。

class LevelLoader
{
private:

    struct ObjectData
    {
        std::string fileName;  // "file_name" フィールドに対応

        std::unique_ptr<WorldTransform> worldTransform;
        std::unique_ptr<Object3d> object3d;

        std::string name;      // "name" フィールドに対応
        std::string type;      // "MESH" などのタイプ
    };

public:

    struct LevelData 
    {
        std::string name;
        std::vector<ObjectData> objects;
    };

private:

    /**
    * \brief  読み込んだlevelDataを実際に使えるように読み込む
    * \param  deserialized : levelData
    */
    std::unique_ptr<LevelData> LoadLevel(nlohmann::json deserialized);

public:

    /**
    * \brief  jsonFileからLevelDataを読み込む
    * \param  fileName : 読み込むfileName
    * \return LevelData
    */
    std::unique_ptr<LevelLoader::LevelData> LoadLevelFromJson(std::string fileName);


};

