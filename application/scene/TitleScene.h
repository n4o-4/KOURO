#pragma once

#include "BaseScene.h"
#include "Kouro.h"
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
#include "SkyDome.h" 

class TitleScene : public BaseScene
{
private:

	std::unique_ptr<Sprite> sprite = nullptr;

	std::unique_ptr<ParticleEmitter> particleEmitter_1 = nullptr;



	

	// 天球
	std::unique_ptr<SkyDome> skyDome_ = nullptr;

	// ライトクラス
	std::unique_ptr<DirectionalLight> directionalLight = nullptr;
	std::unique_ptr<PointLight> pointLight = nullptr;
	std::unique_ptr<SpotLight> spotLight = nullptr;

	//model
	std::unique_ptr<Object3d> player_ = nullptr;
	std::unique_ptr<WorldTransform> playerWorldTransform_ = nullptr;

	//
	std::unique_ptr<Sprite> title_ = nullptr;
	std::unique_ptr<Sprite> start1_ = nullptr;
	std::unique_ptr<Sprite> start2_ = nullptr;
	std::unique_ptr<Sprite> select1_ = nullptr;
	std::unique_ptr<Sprite> select2_ = nullptr;
	std::unique_ptr<Sprite> select3_ = nullptr;
	std::unique_ptr<Sprite> select4_ = nullptr;

	std::unique_ptr<Sprite> operation_ = nullptr;

	bool start = false;
	bool operation = false;
	bool easy = false;
	bool nomal = false;
	bool hard = false;

	int stsrtTime = 120;
	int selectNum = 0;

	bool music = false;

	std::unique_ptr<Audio> audio = nullptr;

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

	void select();

	bool GetEasy() const { return easy; }
	bool GetNomal() const { return nomal; }
	bool GetHard() const { return hard; }
};