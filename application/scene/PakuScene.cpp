#include "PakuScene.h"

void PakuScene::Initialize()
{
	BaseScene::Initialize();


	//skyDome
	ModelManager::GetInstance()->LoadModel("skyDome/skyDome.obj");
	skyDomeObj_ = std::make_unique<Object3d>();
	skyDomeObj_->Initialize(Object3dCommon::GetInstance());
	skyDomeObj_->SetModel("skyDome.obj");
	skyDomeObj_->SetCamera(camera.get());
	skyDome_->Initialize(std::move(skyDomeObj_));
	

}

void PakuScene::Finalize()
{
}

void PakuScene::Update()
{
	skyDome_->Update();
}

void PakuScene::Draw()
{
	skyDome_->Draw();
}
