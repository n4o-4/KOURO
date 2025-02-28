#include "PakuScene.h"



void PakuScene::Initialize()
{
	BaseScene::Initialize();


	directionalLight = std::make_unique<DirectionalLight>();
	directionalLight->Initilaize();

	pointLight = std::make_unique<PointLight>();
	pointLight->Initilize();

	spotLight = std::make_unique<SpotLight>();
	spotLight->Initialize();
	
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Initialize();

	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();


}

void PakuScene::Finalize()
{

	skyDome_.reset();

	ground_.reset();

	enemy_.reset();
}

void PakuScene::Update()
{


	BaseScene::Update();
	skyDome_->Update();
	ground_->Update();
	enemy_->Update();

}

void PakuScene::Draw()
{
	Object3dCommon::GetInstance()->SetView();

	skyDome_->Draw(Camera::GetInstance()->GetViewProjection(),
		*directionalLight.get(),
		*pointLight.get(),
		*spotLight.get());

	ground_->Draw(Camera::GetInstance()->GetViewProjection(),
		*directionalLight.get(),
		*pointLight.get(),
		*spotLight.get());

	enemy_->Draw(Camera::GetInstance()->GetViewProjection(),
		*directionalLight.get(),
		*pointLight.get(),
		*spotLight.get());

}
