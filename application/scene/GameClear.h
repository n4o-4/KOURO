#pragma once

#include "BaseScene.h"
#include"Kouro.h"
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

class GameClear : public BaseScene {

public:

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 描画
	void Draw() override;
	
	void UpdateMoveMwbody();

	void UpdateMissileFlight();


private:

	std::unique_ptr<Audio> audio = nullptr;

	std::unique_ptr<Sprite> clear_ = nullptr;

	std::unique_ptr<Audio> clearBGM_ = nullptr;


	// ライトクラス
	std::unique_ptr<DirectionalLight> directionalLight = nullptr;
	std::unique_ptr<PointLight> pointLight = nullptr;
	std::unique_ptr<SpotLight> spotLight = nullptr;


	// model
	std::unique_ptr<Object3d> mwbody_;
	std::unique_ptr<Object3d> mwdoor_;
	std::unique_ptr<Object3d> mwdish_;
	std::unique_ptr<Object3d> clearModel_;
	std::unique_ptr<Object3d> spawnModel_;
	std::unique_ptr<Object3d> missileModel_;

	// ワールド変換
	std::unique_ptr<WorldTransform> mwbodyTransform_;
	std::unique_ptr<WorldTransform> mwdoorTransform_;
	std::unique_ptr<WorldTransform> mwdishTransform_;
	std::unique_ptr<WorldTransform> clearTransform_;
	std::unique_ptr<WorldTransform> spawnTransform_;
	std::unique_ptr<WorldTransform> missileTransform_;

	// パーティクル
	std::unique_ptr<ParticleEmitter> particleEmitterMissileSmoke_ = nullptr;
	std::unique_ptr<ParticleEmitter> explosionEmitter_ = nullptr;

	//
	float moveTimer_ = 0.0f;
	const float moveDuration_ = 5.0f;
	bool isMoving_ = true;


	//
	float missileTimer_ = 0.0f;
	const float missileDuration_ = 1.0f;
	bool isMissileMoving_ = true;
	bool isMissileVisible_ = true;
};

