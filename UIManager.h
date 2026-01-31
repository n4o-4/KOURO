#pragma once
#include <string>
#include <vector>
#include <map>
#include<memory>

#include "Sprite.h"

class UIManager
{
public:

	void LoadUIFromFile(const std::string& filename);

private:

	std::unordered_map < std::string, std::vector<std::shared_ptr<Kouro::Sprite>>> uiSpriteSets_;

};

