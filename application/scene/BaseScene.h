#pragma once

#include <memory>
#include "SrvManager.h"
#include "Camera.h"

#include "CameraManager.h"

#include "SpriteCommon.h"
#include "Object3dCommon.h"	
#include "LineDrawerBase.h"

#include "Fade.h"

#include "PostEffect.h"

#include "Skybox.h"

class SceneManager;

// \brief BaseScene
// ゲームシーンの基本クラス。
// フェーズ管理（フェードイン、メイン、プレイ、ポーズ、フェードアウト）や、
// シーン内の更新・描画処理の共通処理を提供。
// LineDrawer、CameraManager、Fade、Skyboxなどの共通機能を管理し、
// 派生シーンでのオブジェクト描画やスプライト描画、フェード処理をサポート。

class BaseScene
{
public:

	enum class Phase
	{
		kFadeIn,   // フェードイン

		kMain,     // メイン部

		kPlay,     // ゲームプレイ

		kPose,     // ポーズ

		kFadeOut,  // フェードアウト
	};

public:

	// \brief 
	virtual ~BaseScene() = default;

	// \brief Initialize 初期化
	virtual void Initialize();

	// \brief Finalize 終了
	virtual void Finalize();

	// \brief Update 更新
	virtual void Update();

	// \brief Draw 描画
	virtual void Draw();

	/**
	* \brief  SetSceneManager シーンマネージャーの設定
	* \ param SceneManager シーンマネージャーのポインタ
	*/
	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

	/**
	* \brief  SetSrvManager Srvマネージャーを設定する
	* \param  SrvManager Srvマネージャーのポインタ
	*/
	virtual void SetSrvManager(SrvManager* srvManager) { srvManager_ = srvManager; }

	/**
	* \brief  GetSrvManager Srvマネージャーを取得する
	* \return SrvManager
	*/
	virtual SrvManager* GetSrvManager() { return srvManager_; }

protected:

	virtual void LineDraw();

	virtual void DrawObject();

	virtual void DrawBackgroundSprite();

	virtual void DrawForegroundSprite();

	virtual void DrawFade();

protected:

	SceneManager* sceneManager_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	std::unique_ptr<LineDrawerBase> lineDrawer_ = nullptr;

	std::unique_ptr<CameraManager> cameraManager_ = nullptr;

	std::unique_ptr<Fade> fade_ = nullptr;

	float fadeTime_ = 1.5f;

	Phase phase_ = Phase::kFadeIn;

	bool isContinue = true;

	std::unique_ptr<Skybox> skybox_ = nullptr;

private:


};