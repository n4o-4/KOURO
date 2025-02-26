#include "OkaScene.h"

void OkaScene::Initialize()
{
	BaseScene::Initialize();

	directionalLight = std::make_unique<DirectionalLight>();
	directionalLight->Initilaize();

	pointLight = std::make_unique<PointLight>();
	pointLight->Initilize();

	spotLight = std::make_unique<SpotLight>();
	spotLight->Initialize();

	// プレイヤーを生成
	player_ = std::make_unique<Player>();
	player_->Initialize();
}

void OkaScene::Finalize()
{
	player_.reset();
}

void OkaScene::Update()
{
	BaseScene::Update();

	// プレイヤーの更新
	player_->Update();
}

void OkaScene::Draw()
{
	SpriteCommon::GetInstance()->SetView();

	//sprite->Draw();

	Object3dCommon::GetInstance()->SetView();

	// プレイヤーの描画
	player_->Draw(Camera::GetInstance()->GetViewProjection(),
		*directionalLight.get(),
		*pointLight.get(),
		*spotLight.get());
}
