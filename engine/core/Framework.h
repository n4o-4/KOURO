#pragma once

#include <memory>

#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Audio.h"
#include "SrvManager.h"
#include "TextureManager.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3dCommon.h"
#include "ModelCommon.h"
#include "Object3d.h"
#include "ModelManager.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "ImGuiManager.h"
#include "BaseScene.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"

#include "LineDrawerBase.h"
#include "PostEffect.h"

#include "LeakChecker.h"

#include "GpuParticle.h"
#include "GpuParticleManager.h"

#include <chrono>
#include <string>
#include <wrl.h>

// \brief FrameWork ゲーム全体の基本的な流れ（初期化・更新・描画・終了）を管理するフレームワーククラス。DirectX環境や各種マネージャを統合し、アプリのメインループを制御する。 

class Framework
{
public:

protected:

	std::unique_ptr<WinApp> winApp = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

	std::unique_ptr<SrvManager> srvManager = nullptr;

	std::unique_ptr<ModelCommon> modelCommon = nullptr;

	std::unique_ptr<UavManager> uavManager_ = nullptr;

#ifdef _DEBUG

	std::unique_ptr<ImGuiManager> imGuiManager = nullptr;

#endif
	std::unique_ptr<AbstaractSceneFactory> sceneFactory_ = nullptr;

	
	bool endRequest_ = false;

	std::unique_ptr<LineDrawerBase> lineDrawer_ = nullptr;

	std::unique_ptr<PostEffect> postEffect_ = nullptr;

	std::unique_ptr<GpuParticleManager> gpuParticleManager_ = nullptr;

	GpuParticle* gpuParticle_ = nullptr;

	// フレームごとの時間計測用
	uint64_t frameCount = 0;
	double totalTime = 0.0; // 総時間
	float deltaTime;
	std::chrono::steady_clock::time_point startTime;
	std::chrono::steady_clock::time_point lastTime;
	double elapsedTime = 0.0; 
	double fps = 0.0;
	std::chrono::steady_clock::time_point now;

	const float kDeltaTime_ = 1.0f / 60.0f;

	EngineContext engineContext;

public:

	virtual~Framework() = default;

	// 初期化
	virtual void Initialize();

	// 終了
	virtual void Finalize();

	// 毎フレーム更新
	virtual void Update();

	// 描画
	virtual void Draw();

	virtual void DrawEffect();

	// 終了チェック
	virtual bool IsEndRequest() { return endRequest_; }

	// 実行
	void Run();

	const float GetDeltaTime() const { return kDeltaTime_; }

private:

	void UpdateFPS();
};
