#include "SceneFactory.h"

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
	


	/// �e�l�̃V�[��
	else if (sceneName == "OKA")
	{
		newScene = std::make_unique<OkaScene>();
	}
	else if (sceneName == "PAKU")
	{
		newScene = std::make_unique<PakuScene>();
	}
	else if (sceneName == "MARU")
	{
		newScene = std::make_unique<MaruScene>();
	}
	else if (sceneName == "MIYA")
	{
		newScene = std::make_unique<MiyaScene>();
	}

    return newScene;
}
