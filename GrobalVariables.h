#pragma once
#include <string>
#include <json.hpp>

using json = nlohmann::json;

class GrobalVariables
{
public:

	void SaveFile(const std::string& item,uint32_t value);

	uint32_t LoadFile(const std::string& item, uint32_t defaultValue = 0);

};

