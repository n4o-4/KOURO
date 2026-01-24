#pragma once

#include <dxgidebug.h>

#include "Framework.h"

#include "SceneFactory.h"

// \brief  ゲーム全体

class MyGame : public Kouro::Framework
{
public: //　メンバ関数

	// \brief  Initialize 初期化
	void Initialize() override;

	// \brief  Finalize 終了
	void Finalize() override;

	// \brief  Update 更新
	void Update() override;

	// \brief  Draw 描画
	void Draw() override;

	/**
	* \brief  IsEndRequest 終了要求が来ているか
	* \return 終了要求が来ているか
	*/
	bool IsEndRequest() override { return endRequest_; }

private:

	// シーンファクトリー
	std::unique_ptr<Kouro::AbstractSceneFactory> sceneFactory_ = nullptr;
};