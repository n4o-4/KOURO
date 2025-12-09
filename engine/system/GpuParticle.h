#pragma once
#include "D3D12Context.h"
#include "DirectXCommon.h"
#include "SrvManager.h"
#include "UavManager.h"
#include "Vectors.h"

#include "ModelDatas.h"
#include "ViewProjection.h"

#include "LineModelManager.h"

struct ParticleCS
{
	Vector3 translate; float pad0;
	Vector3 scale;     float pad1;
	float lifeTime;    float pad2[3];
	Vector3 velocity;  float pad3;
	float currentTime; float pad4[3];
	Vector4 color;
};

struct PerView
{
	Matrix4x4 viewProjection;
	Matrix4x4 billboardMatrix;
};

namespace Particle
{
	struct Material
	{
		Vector4 color;
		Matrix4x4 uvTransform;
	};

	struct EmitterSphere
	{
		Vector3 translate;   // �ʒu
		float radius;        // �ˏo���a
		uint32_t count;      // �ˏo��
		float frequency;     // �ˏo�Ԋu
		float frequencyTime; // �ˏo�Ԋu�����p����
		uint32_t emit;       // �ˏo����
	};

	struct PerFrame
	{
		float time;
		float deltaTime;
	};

	struct Transform
	{
		Matrix4x4 matWorld;
	};

	struct LineSegment
	{
		Vector3 start;
		float pad1 = 0.0f;
		Vector3 end;
		float pad2 = 0.0f;
	};
}

// \brief GpuParticle
// GPU��Ńp�[�e�B�N����Ǘ��E�`�悷��V���O���g���N���X�B
// �ő�p�[�e�B�N������Ǘ����AComputeShader�ɂ�鏉�����A�����A�X�V������s���B
// �܂��A����p�[�e�B�N��(LineSegment)�̔�����`���T�|�[�g�B
// ����Ŋe�탊�\�[�X�i�p�[�e�B�N���A�J�E���^�[�A�t���X�g�A���_�A�}�e���A���A�G�~�b�^�[�Atransform�AlineSegment�Ȃǁj��Ǘ��B
// PipelineSet��p���āAGPU����Compute��Graphics�p�C�v���C����\�����A�����I�ȑ�ʃp�[�e�B�N���`���\�ɂ���B

class GpuParticle
{
public:

	// �ő�p�[�e�B�N����
	const uint32_t kMaxParticleCount = 524288;

public:

	static GpuParticle* GetInstance();

	/**
	* \brief  Initialize ������
	* \param  dxCommon DirectXCommon�̃|�C���^
	* \param  srvManager SrvManager�̃|�C���^
	* \param  uavManagedr UavManager�̃|�C���^
	*/
	void Initialize(DirectXCommon* dxCommon,SrvManager* srvManager,UavManager* uavManagedr);

	// \brief  Finalize �I��
	void Finalize();

	// \brief  Update �X�V
	void Update(ViewProjection viewProjection);

	void Draw();

	void SetPerFrame(float time, float deltaTime) { perFrame_->time = time, perFrame_->deltaTime = deltaTime; }

	void CreateLineSegment(std::string filePath);

	void LineEmit(std::string groupName, Matrix4x4 world);

private: // ����J�����o�֐�

	// \brief  CreateResourc0e ���\�[�X�̐���
	void CreateResource();

	// \brief  CreateVertexResource ���_���\�[�X�̐���
	void CreateVertexResource();

	// \brief  CreateVertexBufferView ���_�o�b�t�@�r���[�̐���
	void CreateVertexBufferView();

	// \brief  CreatePerViewResource �p�[�r���\���\�[�X�̐���
	void CreatePerViewResource();

	// \brief  CreateMaterialResource �}�e���A�����\�[�X�̐���
	void CreateMaterialResource();

	// \brief  CreateEmitterResource �G�~�b�^�[���\�[�X�̐���
	void CreateEmitterResource();

	// \brief  CreatePerFrameResource perFrame���\�[�X�̐���
	void CreatePerFrameResource();

	// \brief  CreateTransformResource transform���\�[�X�̐���
	void CreateTransformResource();

	// \brief  CreateLineSegmentResource lineSegment���\�[�X�̐���
	void CreateLineSegmentResource();

	//
	void CreateLineCountResource();

	/// Compute

	// initialize
	void CreateInitializePipelineSet();

	// emit
	void CreateEmitPipelineSet();

	void CreateLineEmitPipelineSet();

	// update
	void CreateUpdatePipelineSet();

	/// Graphics

	// \brief  CreateGraphicsRootSignature �`��p���[�g�V�O�l�`���̍쐬
	void CreateGraphicsRootSignature();

	// \brief  CreateGraphicsPipelineState �`��p�p�C�v���C���X�e�[�g�̍쐬
	void CreateGraphicsPipelineState();

	// \brief  CreateComputePipelineState Compute�p�C�v���C���X�e�[�g�̍쐬
	void CreateComputePipelineState(PipelineSet* pipelineSet,std::string shaderPath);

	// \brief  CreatePipelineSet �p�C�v���C���Z�b�g�̍쐬
	void CreatePipelineSet();

private:

	static std::unique_ptr<GpuParticle> instance;

	friend std::unique_ptr<GpuParticle> std::make_unique<GpuParticle>();
	friend std::default_delete<GpuParticle>;

	GpuParticle() = default;
	~GpuParticle() = default;
	GpuParticle(GpuParticle&) = delete;
	GpuParticle& operator= (GpuParticle&) = delete;


	DirectXCommon* dxCommon_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	UavManager* uavManager_ = nullptr;

	MaterialData materialData_;

	ParticleModel::ModelData modelData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> particleResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> counterResource_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> freeListResource_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> perViewResource_ = nullptr;
	PerView* perView_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;
	Particle::Material* material_ = nullptr;

	// �G�~�b�^�[
	Microsoft::WRL::ComPtr<ID3D12Resource> emitterResource_ = nullptr;
	Particle::EmitterSphere* emitter_ = nullptr;

	// perFrame
	Microsoft::WRL::ComPtr<ID3D12Resource> perFrameResource_ = nullptr;
	Particle::PerFrame* perFrame_ = nullptr;

	// transform
	Microsoft::WRL::ComPtr<ID3D12Resource> transformResource_ = nullptr;
	Particle::Transform* transform_ = nullptr;

	// lineSegment
	Microsoft::WRL::ComPtr<ID3D12Resource> lineSegmentResource_ = nullptr;
	Particle::LineSegment* lineSegment_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> lineCountResource_ = nullptr;
	uint32_t* lineCount_ = nullptr;

	uint32_t srvIndex_ = 0;

	uint32_t uavIndex_ = 0;

	// ComputeShader�p(InitializeParticle)
	std::unique_ptr<PipelineSet> initializePipelineSet_;
	
	// ComputeShader�p(EmitParticle)
	std::unique_ptr<PipelineSet> emitPipelineSet_;

	// ComputeShader�p(���㔭��)
	std::unique_ptr<PipelineSet> lineEmitPipelineSet_;

	// ComputeShader�p(UpdateParticle)
	std::unique_ptr<PipelineSet> updatePipelineSet_;

	// �`��p
	std::unique_ptr<PipelineSet> graphicsPipelineSet_;

	const float kDeltaTime_ = 1.0f / 60.0f;

	std::vector<Particle::LineSegment> lineSegments_;

	uint32_t lineSegmentSrvIndex_ = 0;

};

