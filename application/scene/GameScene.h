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

#include "WorldTransform.h"

#include "AnimationManager.h"

class GameScene : public BaseScene
{
public:

	std::unique_ptr<Sprite> sprite = nullptr;

	std::unique_ptr<Object3d> object3d = nullptr;

	std::unique_ptr<WorldTransform> objectTransform = nullptr;

	std::unique_ptr<Camera> camera = nullptr;

	std::unique_ptr<DirectionalLight> directionalLight = nullptr;

	std::unique_ptr<PointLight> pointLight = nullptr;

	std::unique_ptr<SpotLight> spotLight = nullptr;

	std::unique_ptr<AnimationManager> animationManager = nullptr;

private:

public: // メンバ関数

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 描画
	void Draw() override;

	
};