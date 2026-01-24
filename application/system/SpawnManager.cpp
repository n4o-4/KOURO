#include "SpawnManager.h"
#include <iostream>

std::vector<Kouro::Vector3> SpawnManager::LoadFile(const std::string fileName)
{
    const std::string filePath = directoryPath + fileName;

    try {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open JSON file: " + filePath);
        }

        nlohmann::json j;
        file >> j;

        std::vector<Kouro::Vector3> goalOffsets;

        for (auto& e : j["EnemyGroup1"]["enemis"]) {
            Kouro::Vector3 goalOffset;
            goalOffset.x = e["goalOffset"][0].get<float>();
            goalOffset.y = e["goalOffset"][1].get<float>();
            goalOffset.z = e["goalOffset"][2].get<float>();
            goalOffsets.push_back(goalOffset);
        }

        return goalOffsets;
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error at byte " << e.byte << ": " << e.what() << std::endl;
        return {};
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading JSON: " << e.what() << std::endl;
        return {};
    }
}