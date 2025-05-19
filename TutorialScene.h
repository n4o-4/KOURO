#pragma once
#include "BaseScene.h"

#include "Sprite.h"
#include "Audio.h"


class TutorialScene : public BaseScene {
private:
	std::unique_ptr<Sprite> tutorialImage_ = nullptr;
	std::unique_ptr<Audio> audio_ = nullptr;

public:
	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
};
