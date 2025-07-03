#pragma once

#pragma once
#include "Vectors.h"
#include "Matrixs.h"
#include "ModelCommon.h"
#include <fstream>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "TextureManager.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ModelDatas.h"

class Skybox
{
public:

	struct Direction
	{
		Vector3 direction; //!< �����x�N�g��
		float padding[1]; //!< ���x
	};

public: // ���J�����o�֐�
	
	void Initialize(const std::string& filePath);

	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

private:

	void CreateRootSignature();

	void CreateGraphicsPipeline();



private:

	DirectXCommon* dxCommon_; //!< DirectX���ʃN���X�ւ̃|�C���^

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_; //!< ���_���\�[�X

	VertexData* vertexData_; //!< ���_�f�[�^


	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	
	//ID3DBlob* signatureBlob = nullptr;
	Microsoft::WRL::ComPtr< ID3DBlob> signatureBlob = nullptr;
	//ID3DBlob* errorBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	Microsoft::WRL::ComPtr< ID3D12RootSignature> rootSignature;

	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_; //!< �}�e���A�����\�[�X

	Material* materialData_; //!< �}�e���A���f�[�^

	Microsoft::WRL::ComPtr<ID3D12Resource> directionResource_; //!< ���_���\�[�X

	VertexData* directionData_; //!< ���_�f�[�^

	std::unique_ptr<WorldTransform> worldTransform_; //!< ���[���h�g�����X�t�H�[��
};

