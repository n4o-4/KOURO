#include "Sprite.h"
#include "SpriteRenderer.h"
#include "TextureManager.h"

namespace Kouro
{
	void Sprite::Initialize(GpuResourceUtils* utils, std::string textureFilePath)
	{
		resourceUtils_ = utils;

		textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);

		// バッファリソース

		CreateMaterialData();

		CreateTransformationMatrixData();

		AdjustTextureSize(textureFilePath);

		textureFilePath_ = textureFilePath;
	}

	void Sprite::Update()
	{
		float left = 0.0f - anchorPoint.x;
		float right = 1.0f - anchorPoint.x;

		float top = 0.0f - anchorPoint.y;
		float bottom = 1.0f - anchorPoint.y;

		// 左右反転
		if (isFlipX_) {
			left = -left;
			right = -right;
		}

		// 上下反転
		if (isFlipY_) {
			top = -top;
			bottom = -bottom;
		}

		const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(textureFilePath_);

		float tex_left = textureLeftTop.x / metadata.width;
		float tex_right = (textureLeftTop.x + textureSize.x) / metadata.width;
		float tex_top = textureLeftTop.y / metadata.height;
		float tex_bottom = (textureLeftTop.y + textureSize.y) / metadata.height;

		// 1枚目の三角形
		vertexData_[0].position = { left,bottom,0.0f,1.0f }; // 左下
		vertexData_[0].texcoord = { tex_left,tex_bottom };
		vertexData_[0].normal = { 0.0f,0.0f,-1.0f };

		vertexData_[1].position = { left,top,0.0f,1.0f }; // 左上
		vertexData_[1].texcoord = { tex_left,tex_top };
		vertexData_[1].normal = { 0.0f,0.0f,-1.0f };

		vertexData_[2].position = { right,bottom,0.0f,1.0f }; // 右下
		vertexData_[2].texcoord = { tex_right,tex_bottom };
		vertexData_[2].normal = { 0.0f,0.0f,-1.0f };

		vertexData_[3].position = { right,top,0.0f,1.0f }; // 右上
		vertexData_[3].texcoord = { tex_right,tex_top };
		vertexData_[3].normal = { 0.0f,0.0f,-1.0f };

		transform.scale = { size.x,size.y };

		transform.translate = { position.x,position.y,0.0f };

		transform.rotate = { 0.0f,0.0f,rotation };

		Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
		Matrix4x4 viewMatrix = MakeIdentity4x4();
		Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, WinApp::kClientWidth, WinApp::kClientHeight, 0.0f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		transformationMatrixData->WVP = worldViewProjectionMatrix;
		transformationMatrixData->World = worldMatrix;

		// UVTransform用の行列
		Matrix4x4 uvTransformMatrix = MakeAffineMatrix(uvTransform.scale, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
		uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransform.rotate.z));
		uvTransformMatrix = Multiply(uvTransformMatrix, MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, uvTransform.translate));
		materialData->uvTransform = uvTransformMatrix;
	}



	void Sprite::Draw()
	{

		// 背景描画のフラグが立っている場合	
		if (spriteCommon->GetIsDrawBackground())
		{
			transform.translate.z = -100.0f;

			Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
			Matrix4x4 viewMatrix = MakeIdentity4x4();
			Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, WinApp::kClientWidth, WinApp::kClientHeight, 0.0f, 100.0f);
			Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
			transformationMatrixData->WVP = worldViewProjectionMatrix;
			transformationMatrixData->World = worldMatrix;
		}

		// 前景描画のフラグが立っている場合
		else if (spriteCommon->GetIsDrawForeground())
		{
			transform.translate.z = 0.000000f;

			Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
			Matrix4x4 viewMatrix = MakeIdentity4x4();
			Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, WinApp::kClientWidth, WinApp::kClientHeight, 0.0f, 100.0f);
			Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
			transformationMatrixData->WVP = worldViewProjectionMatrix;
			transformationMatrixData->World = worldMatrix;
		}

		spriteCommon->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);  // VBVを設定

		spriteCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource_.Get()->GetGPUVirtualAddress());

		spriteCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationResource_.Get()->GetGPUVirtualAddress());

		spriteCommon->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureFilePath_));

		spriteCommon->GetDxCommon()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	}

	
	void Sprite::CreateVertexData()
	{
		vertexResource_ = resourceUtils_->CreateBufferResource(sizeof(SpriteContext::VertexData) * 4);
		vertexResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	}

	void Sprite::CreateMaterialData()
	{
		materialResource_ = resourceUtils_->CreateBufferResource(sizeof(Material));

		materialResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

		materialData_->color = { 1.0f,1.0f,1.0f,1.0f };

		materialData_->enableLighting = false;

		materialData_->uvTransform = MakeIdentity4x4();
	}

	void Sprite::CreateTransformationMatrixData()
	{
		transformationResource_ = resourceUtils_->CreateBufferResource(sizeof(TransformationMatrix));

		// データを書き込むためのアドレスを取得
		transformationResource_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

		//　単位行列を書き込んでおく
		transformationMatrixData->WVP = MakeIdentity4x4();zz
		transformationMatrixData->World = MakeIdentity4x4();
	}

	void Sprite::AdjustTextureSize(std::string textureFilePath)
	{
		// テクスチャのメタデータを取得
		const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(textureFilePath);

		// テクスチャサイズをメタデータから設定
		textureSize.x = static_cast<float>(metadata.width);
		textureSize.y = static_cast<float>(metadata.height);
	}
}