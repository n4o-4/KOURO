#include "BaseBullet.h"

void BaseBullet::Initialize(Model* model)
{
	///========================================
	/// オブジェクト3D

	// 生成と初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(Object3dCommon::GetInstance());

	// モデルを設定
	object3d_->SetModel(model);

	// ローカル行列の初期化
	object3d_->SetLocalMatrix(MakeIdentity4x4());



	///========================================
	/// ワールドトランスフォーム

	// 生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();


	worldTransform_->useQuaternion_ = false;
	// 初期値の設定


	///========================================
	/// その他変数

	// 速度の初期化
	velocity_ = { 0.0f,0.0f,0.0f };

	// 有効フラグの初期化
	isActive_ = true;
}

void BaseBullet::Update()
{
	///========================================
	/// ワールドトランスフォーム

	if (worldTransform_->useQuaternion_)
	{
		worldTransform_->quaternionTransform.translate += velocity_;
	}
	else
	{
		worldTransform_->transform.translate += velocity_;
	}

	// 更新
	worldTransform_->UpdateMatrix();
}

void BaseBullet::Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	///========================================
	/// オブジェクト3D

	// 描画
	object3d_->Draw(*worldTransform_.get(), camera_->GetViewProjection(), directionalLight, pointLight, spotLight);
}
