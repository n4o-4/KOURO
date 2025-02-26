#pragma once

#include "AbstractSceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"

// このゲーム用のシーン工場
class SceneFactory : public AbstaractSceneFactory
{
public:


	std::unique_ptr<BaseScene> CreateScene(const std::string& sceneName) override;

};