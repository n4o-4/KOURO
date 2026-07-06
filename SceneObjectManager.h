#pragma once
#include <memory>
#include <vector>

// engine
#include "BaseCharacter.h"

class SceneObjectManager
{
public:
	void Initialize();

	void Update();

	void Draw();

	void AddCharacter(std::unique_ptr<BaseCharacter> character);

private:
	std::vector<std::unique_ptr<BaseCharacter>> characters_;
};

