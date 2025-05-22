#include "TitleScene.h"

void TitleScene::Initialize()
{
	BaseScene::Initialize();

	TextureManager::GetInstance()->LoadTexture("Resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("Resources/fruit_suika_red.png");

	/*sprite = std::make_unique<Sprite>();

	sprite->Initialize(SpriteCommon::GetInstance(), "Resources/monsterBall.png");

	sprite->SetTexSize({ 1200.0f,600.0f });

	sprite->SetAnchorPoint({ 0.5f,0.5f });

	sprite->SetPosition({ 640.0f,360.0f });

	sprite->SetSize({ 1280.0f,720.0f });*/

	ModelManager::GetInstance()->LoadModel("axis.obj");

	/*object3d = std::make_unique<Object3d>();

	object3d->Initialize(Object3dCommon::GetInstance());

	object3d->SetModel("axis.obj");

	object3d->SetCamera(camera.get());*/

	ParticleManager::GetInstance()->CreateParticleGroup("Particle_1", "Resources/circle.png",ParticleManager::ParticleType::Normal);


	particleEmitter_1 = std::make_unique<ParticleEmitter>();
	particleEmitter_1->Initialize("Particle_1");

	AudioManager::GetInstance()->Initialize();
	AudioManager::GetInstance()->SoundLoadFile("Resources/Alarm01.wav");

    audio = std::make_unique<Audio>();
	audio->Initialize();
	audio->SoundPlay("Resources/Alarm01.wav",999);

	audio->SetVolume(0.5f);

	//title
	TextureManager::GetInstance()->LoadTexture("Resources/scene/title.png");
	title_ = std::make_unique<Sprite>();
	title_->Initialize(SpriteCommon::GetInstance(), "Resources/scene/title.png");
	//title_->SetAnchorPoint({ 0.5f,0.5f });
	title_->SetTexSize({ 1280.0f,720.0f });
	title_->SetSize({ 1280.0f,720.0f });
	title_->SetPosition({ 0.0f,0.0f });
	
}

void TitleScene::Finalize()
{
	BaseScene::Finalize();

	//audio->SoundStop("Resources/Spinning_World.mp3");
}

void TitleScene::Update()
{
	switch (phase_)
	{
	case Phase::kFadeIn:

		if (fade_->IsFinished())
		{
			Input::GetInstance()->SetIsReception(true);
			phase_ = Phase::kMain;
		}

		break;
	case Phase::kMain:

		if (Input::GetInstance()->Triggerkey(DIK_RETURN) || Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A))
		{
			fade_->Start(Fade::Status::FadeOut, fadeTime_);
			phase_ = Phase::kFadeOut;
		}

		break;
	case Phase::kFadeOut:

		if (fade_->IsFinished())
		{
			SceneManager::GetInstance()->ChangeScene("GAME");

			return;
		}

		break;

	case Phase::kPlay:
		break;
	case Phase::kPose:
		break;
	}

	BaseScene::Update();

	//sprite->Update();

	/*Vector3 rotato = object3d->GetRotation();

	rotato.y += 0.02f;

	object3d->SetRotation(rotato);

	Vector3 translate = object3d->GetTranslate();

	translate.z += 0.1f;

	object3d->SetTranslate(translate);

	object3d->Update();*/

	ParticleManager::GetInstance()->Update();
	particleEmitter_1->Update();

	//title
	title_->Update();

}

void TitleScene::Draw()
{
	BaseScene::Draw();

	DrawBackgroundSprite();
	/// 背景スプライト描画

	//title
	title_->Draw();

	DrawObject();
	/// オブジェクト描画	


	DrawForegroundSprite();	
	/// 前景スプライト描画	

	fade_->Draw();
	ParticleManager::GetInstance()->Draw("Resources/circle.png");

}
