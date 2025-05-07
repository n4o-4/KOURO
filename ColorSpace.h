#pragma once
#include "BaseEffect.h"

namespace ColorSpaceShader
{
	struct Material
	{
		float hue;
		float saturation;
		float value;
		float padding[1];
	};
}

class ColorSpace : public BaseEffect
{
public:

	// ������
	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager) override;

	// �X�V
	void Update() override;

	// �`��
	void Draw(uint32_t renderTargetIndex, uint32_t renderResourceIndex) override;

	// ���\�[�X�̉��
	void Reset() override { resource_.Reset(); }

private:

	void CreatePipeline();


	void CreateRootSignature(Pipeline* pipeline);


	void CreatePipeLineState(Pipeline* pipeline);

	// �}�e���A���̐���
	void CreateMaterial();

	void DrawImGui() override;

public:

	float hue = 0.0f;
	float saturation = 0.0f;
	float value = 0.0f;	

private: // �����o�ϐ�

	// ���\�[�X
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

	// �|�C���^�[
	ColorSpaceShader::Material* data_ = nullptr;

};

