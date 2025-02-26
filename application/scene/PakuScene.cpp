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
}

void PakuScene::Finalize()
{

	
	skyDome_.reset();
}

void PakuScene::Update()
{


	BaseScene::Update();
	skyDome_->Update();
}

void PakuScene::Draw()
{
	Object3dCommon::GetInstance()->SetView();

	skyDome_->Draw(Camera::GetInstance()->GetViewProjection(),
		*directionalLight.get(),
		*pointLight.get(),
		*spotLight.get());
}
