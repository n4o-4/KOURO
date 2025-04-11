﻿#include "SceneFactory.h"

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
    std::unique_ptr<BaseScene> newScene = nullptr;

    if (sceneName == "TITLE")
    {
        newScene = std::make_unique<TitleScene>();
    }
    else if (sceneName == "GAME")
    {
        newScene = std::make_unique<GameScene>();
    }
    else if (sceneName == "CLEAR") {
        newScene = std::make_unique<GameClear>();
    }
    else if (sceneName == "OVER") {
        newScene = std::make_unique<GameOver>();
    }


	/// 各個人のシーン

	

    return newScene;
}
