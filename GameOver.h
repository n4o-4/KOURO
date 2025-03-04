#pragma once

#include "BaseScene.h"

#include "audio.h"
#include "Input.h"
#include "Sprite.h"
#include "SpriteCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Object3d.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "SceneManager.h"
#include "Audio.h"

class GameOver : public BaseScene {


public: 

	// ������
	void Initialize() override;
	// �I��
	void Finalize() override;
	// ���t���[���X�V
	void Update() override;
	// �`��
	void Draw() override;



private:

	std::unique_ptr<Audio> audio = nullptr;

	std::unique_ptr<Sprite> over_ = nullptr;



};

