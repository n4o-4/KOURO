#pragma once
#include "BaseScene.h"

#include "Kouro.h"
#include "SkyDome.h"

class PakuScene : public BaseScene
{
public: // メンバ関数

	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 描画
	void Draw() override;


private:

	std::unique_ptr<SkyDome> skyDome_ = nullptr;

	std::unique_ptr<WorldTransform> objectTransform = nullptr;

	std::unique_ptr<Camera> camera = nullptr;

	std::unique_ptr<DirectionalLight> directionalLight = nullptr;

	std::unique_ptr<PointLight> pointLight = nullptr;

	std::unique_ptr<SpotLight> spotLight = nullptr;
};

