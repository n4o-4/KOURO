#pragma once
#include "Vectors.h"
#include "Matrixs.h"
#include <fstream>
#include <string>
#include "Object3dCommon.h"
#include "Model.h"
#include "WinApp.h"
#include "ModelManager.h"
#include "Camera.h"

#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

class Object3d
{
private:

	struct Transform
	{
		Vector3 scale;
		Vector3 rotate;
		Vector3 translate;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
	
	};

public: // メンバ関数
	// 初期化
	void Initialize(Object3dCommon* object3dCommon);

	// 更新
	void Update();

	// 描画
	void Draw(WorldTransform worldTransform,ViewProjection viewProjection, DirectionalLight directionalLight ,PointLight pointLight,SpotLight spotLight);

	void SetModel(const std::string& filePath);

	// setter
	void SetScale(const Vector3& scale) { transform.scale = scale; }
	void SetRotation(const Vector3& rotation) { transform.rotate = rotation; }
	void SetTranslate(const Vector3& translate) { transform.translate = translate; }

	const Vector3& GetScale() { return transform.scale; }
	const Vector3& GetRotation() { return transform.rotate;}
	const Vector3& GetTranslate() { return transform.translate; }

	void SetCamera(Camera* camera) { this->camera = camera; }

	// setter
	void SetModel(Model* model) { this->model = model; }

	void SetLocalMatrix(Matrix4x4 localMatrix) { this->localMatrix = localMatrix; }


private:

	

private:
	Object3dCommon* object3dCommon = nullptr;

	Matrix4x4 localMatrix;

	//ModelData modelData;

	// バッファリソース
	/*Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;

	VertexData* vertexData = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;

	Material* materialData = nullptr;*/

	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource;

	TransformationMatrix* transformationMatrixData = nullptr;

	Transform transform;

	Model* model = nullptr;

	Camera* camera = nullptr;
};

