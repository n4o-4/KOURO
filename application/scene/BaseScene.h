#pragma once

#include <memory>
#include "SrvManager.h"
#include "Camera.h"

class SceneManager;

// シーン基底クラス
class BaseScene
{
public:

	virtual ~BaseScene() = default;

	// 初期化
	virtual void Initialize();

	// 終了
	virtual void Finalize();

	// 更新
	virtual void Update();

	// 描画
	virtual void Draw();

	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

	virtual void SetSrvManager(SrvManager* srvManager) { srvManager_ = srvManager; }

	virtual void SetCamera(Camera* camera) { camera_ = camera; }

	virtual SrvManager* GetSrvManager() { return srvManager_; }

	virtual Camera* GetCamera() { return camera_; }

protected:

	SceneManager* sceneManager_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	Camera* camera_ = nullptr;
};