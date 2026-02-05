#include "Object3d.h"
#include "MyMath.h"
#include "TextureManager.h"
#include "ModelDatas.h"
#include "imgui.h"

namespace Kouro
{
	void Object3d::Initialize(Object3dCommon* object3dCommon)
	{
		this->object3dCommon = object3dCommon;

		transformationResource = this->object3dCommon->GetDxCommon()->CreateBufferResource(sizeof(TransformationMatrix));

		// データを書き込むためのアドレスを取得
		transformationResource.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

		//　単位行列を書き込んでおく
		transformationMatrixData->WVP = MakeIdentity4x4();

		this->camera = object3dCommon->GetDefaultCamera();

		transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

		materialResource_ = object3dCommon->GetDxCommon()->CreateBufferResource(sizeof(Material));

		// 書き込むためのアドレスを取得
		materialResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

		materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		materialData_->enableLighting = false;

		materialData_->uvTransform = MakeIdentity4x4();

		materialData_->shininess = kDefaultShininess;

		materialData_->specularColor = { 1.0f,1.0f,1.0f };
	}

	void Object3d::Update()
	{
		// ワールド行列の計算
		Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
		Matrix4x4 worldViewProjectionMatrix;

#ifdef _DEBUG

#endif

		if (camera) {


		}
		else {

			worldViewProjectionMatrix = worldMatrix;
		}
	}

	void Object3d::Draw(WorldTransform worldTransform, ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
	{
		// ワールド行列を計算
		//worldTransform.matWorld = localMatrix * worldTransform.matWorld;

		// 行列の転送
		worldTransform.TransferMatrix();

		object3dCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());

		object3dCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, viewProjection.GetViewProjectionResource()->GetGPUVirtualAddress());

		object3dCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directionalLight.GetDirectionalLightResource()->GetGPUVirtualAddress());

		object3dCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(5, pointLight.GetPointLightResource()->GetGPUVirtualAddress());

		object3dCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(6, spotLight.GetSpotLightResource()->GetGPUVirtualAddress());

		// モデルの描画
		if (model) {
			model->Draw(worldTransform);
		}
	}

	void Object3d::SetModel(const std::string& filePath)
	{
		// モデルの取得して設定
		model = ModelManager::GetInstance()->FindModel(filePath);
	}
}