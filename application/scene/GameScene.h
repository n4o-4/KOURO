#pragma once

#include "BaseScene.h"

#include "Kouro.h"

#include "SkyDome.h"

#include "Player.h"

#include "SkyDome.h"

#include "Ground.h"

#include "Enemy.h"

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

	std::unique_ptr<Player> player_ = nullptr;

	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	std::unique_ptr<SkyDome> skyDome_ = nullptr;

	std::unique_ptr<Ground> ground_ = nullptr;

	std::unique_ptr<Enemy> enemy_ = nullptr;

private:

	//std::unique_ptr<SkyDome> skyDome_ = nullptr;
	//std::unique_ptr<Object3d> skyDomeObj_ = nullptr;


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