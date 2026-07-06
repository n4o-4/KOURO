#include "SceneObjectManager.h"

void SceneObjectManager::Update()
{
	for (auto& character : characters_)
	{
		if (!character->IsActive()) continue;

		character->Update();
	}

	std::erase_if(characters_, [](const auto& character)
	{
		return !character->IsValid();
	});
}

void SceneObjectManager::Draw()
{
	for(auto& character : characters_)
	{
		if(!character->IsActive())
		{
			continue;
		}

		character->Draw();
	}
}

void SceneObjectManager::AddCharacter(std::unique_ptr<BaseCharacter> character)
{
	// キャラクターを追加
	characters_.push_back(std::move(character));
}
