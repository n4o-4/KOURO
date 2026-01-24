#pragma once
#include <fstream>
#include "json.hpp"
#include "Vectors.h"

class SpawnManager
{
public:

	std::vector<Kouro::Vector3> LoadFile(const std::string fileName);

private:

	const std::string directoryPath = "Resources/";

};

