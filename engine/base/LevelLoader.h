#pragma once
#include <string>
#include <fstream>
#include <cassert>
#include <externals/json.hpp>
#include <regex>

#include "Kouro.h"
#include "Structs.h"



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

    std::unique_ptr<LevelData> LoadLevel(nlohmann::json deserialized);

public:

    std::unique_ptr<LevelLoader::LevelData> LoadLevelFromJson(std::string fileName);


};

