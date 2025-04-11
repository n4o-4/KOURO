#pragma once

#include "BaseScene.h"
#include "Kouro.h"
//========================================
// アプリケーション

#include "FollowCamera.h"
#include <random>
#include "LineDrawerBase.h"



class GameScene : public BaseScene
{
private:
	
	//========================================
	// ライトクラス
	std::unique_ptr<DirectionalLight> directionalLight = nullptr;
	std::unique_ptr<PointLight> pointLight = nullptr;
	std::unique_ptr<SpotLight> spotLight = nullptr;
	//========================================
	// アニメーションマネージャ
	std::unique_ptr<AnimationManager> animationManager = nullptr;
	//========================================
	// ゲームの状態
	// ゲームクリア
	bool isGameClear_ = false;
	// ゲームオーバー
	bool isGameOver_ = false;
	// コンティニュー
	bool isContinue_ = true;
	
	// 待機フラグ
    bool isWaiting_ = false;
	// 待機時間
    int32_t waitTimer_ = 0;

	std::unique_ptr<LineDrawerBase> lineDrawer_ = nullptr;	

	Dissolve* dissolve_ = nullptr;

	std::unique_ptr<Object3d> ground_ = nullptr;

	std::unique_ptr<WorldTransform> transform_ = nullptr;

	std::unique_ptr<ParticleEmitter> emitter_ = nullptr;

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

private: //静的メンバ関数

private:

};