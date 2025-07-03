#include "PlayerBullet.h"

void PlayerBullet::Initialize(Model* model,Vector3 spawnPos)
{
	
	// �e�N���X�̏�����

	BaseBullet::Initialize(model,spawnPos);
	
	// Collider�̏������Ɛݒ�

	SphereCollider::Initialize(worldTransform_.get());
	SetCollisionAttribute(0b1); // �R���W����������ݒ�
	SetCollisionMask(0b1 << 1);
	SetSphere(Sphere({}, 1.0f));

	// �p�[�e�B�N���G�~�b�^�[�̏�����

	emitter_ = std::make_unique<ParticleEmitter>();

	emitter_ = std::make_unique<ParticleEmitter>();
	emitter_->Initialize("plane_Particle");
	emitter_->SetPosition({ 0.0f,2.0f,0.0f });
	emitter_->SetParticleCount(100);
	emitter_->SetVelocityRange({ {-20.0f,-20.0f,-20.0f },{20.0f,20.0f,20.0f} });
	emitter_->SetStartScaleRange({ {0.3f,0.3f,0.3f},{0.3f,0.3f,0.3f} });
	emitter_->SetFinishScaleRange({ {0.1f,0.1f,0.1f},{0.1f,0.1f,0.1f} });
	emitter_->SetStartColorRange({ {0.98f,0.37f,0.01f,1.0f}, {1.0f,0.72f,0.19f,1.0f} });
	emitter_->SetFinishColorRange({ {0.98f,0.0f,0.0f,1.0f}, {1.0f,0.72f,0.19f,1.0f} });
	emitter_->SetLifeTimeRange({ 0.5f,0.8f });
	emitter_->SetFrequency(1.0f);
}

void PlayerBullet::Update()
{
	// �e�N���X�̍X�V

	BaseBullet::Update();

	// Collider�̍X�V

	SphereCollider::Update();
}

void PlayerBullet::Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	// �e�N���X�̕`��

	BaseBullet::Draw(directionalLight, pointLight, spotLight);
}

void PlayerBullet::OnCollisionEnter(BaseCollider* other)
{
	emitter_->SetPosition(worldTransform_->transform.translate);
	emitter_->Emit();
}

void PlayerBullet::OnCollisionStay(BaseCollider* other)
{
}

void PlayerBullet::OnCollisionExit(BaseCollider* other)
{
}
