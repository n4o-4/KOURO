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

// \brief Object3d
// 3Dオブジェクトを表すクラス。
// モデルの設定、スケール・回転・平行移動の設定、描画、ライティングやマテリアルの設定を行う。

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

	/**
	* \brief  初期化
	* \param  object3dCommon Object3dCommonへのポインタ
	*/
	void Initialize(Object3dCommon* object3dCommon);

	/// \brief 更新
	void Update();

	/**
	* \brief  描画
	* \param  worldTransform ワールド変換情報
	* \param  viewProjection ビュー・プロジェクション情報
	* \param  directionalLight ディレクショナルライト情報
	* \param  pointLight ポイントライト情報
	* \param  spotLight スポットライト情報
	*/
	void Draw(WorldTransform worldTransform, ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

	/**
	* \brief  モデルの設定
	* \param  filePath モデルファイルのパス
	*/
	void SetModel(const std::string& filePath);

	/**
	* \brief  モデルの設定
	* \param  model モデルへのポインタ
	*/
	void SetModel(Model* model) { this->model = model; }

	/**
	* \brief  スケールの設定
	* \param  scale スケール値
	*/
	void SetScale(const Vector3& scale) { transform.scale = scale; }

	/**
	* \brief  回転の設定
	* \param  rotation 回転値
	*/
	void SetRotation(const Vector3& rotation) { transform.rotate = rotation; }

	/**
	* \brief  平行移動の設定
	* \param  translate 平行移動値
	*/
	void SetTranslate(const Vector3& translate) { transform.translate = translate; }

	/**
	* \brief  スケールの取得
	* \return スケール値
	*/
	const Vector3& GetScale() { return transform.scale; }

	/**
	* \brief  回転の取得
	* \return 回転値
	*/
	const Vector3& GetRotation() { return transform.rotate; }

	/**
	* \brief  平行移動の取得
	* \return 平行移動値
	*/
	const Vector3& GetTranslate() { return transform.translate; }

	/**
	* \brief  カメラの設定
	* \param  camera カメラへのポインタ
	*/
	void SetCamera(Camera* camera) { this->camera = camera; }

	/**
	* \brief  ローカル行列の設定
	* \param  localMatrix ローカル行列
	*/
	void SetLocalMatrix(Matrix4x4 localMatrix) { this->localMatrix = localMatrix; }

	/**
	* \brief  色の設定
	* \param  color 色
	*/
	void SetColor(Vector4 color) { materialData_->color = color; }

	/**
	* \brief  ライティングの有効化設定
	* \param  enable ライティングを有効にするかどうか
	*/
	void SetEnableLighting(bool enable) { materialData_->enableLighting = enable; }

	/**
	* \brief  環境マップの有効化設定
	* \param  enable 環境マップを有効にするかどうか
	*/
	void SetEnableEnvironmentMap(bool enable) { materialData_->enableEnvironmentMap = enable; }
private:

	

private:
	Object3dCommon* object3dCommon = nullptr;

	Matrix4x4 localMatrix;

	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource;

	TransformationMatrix* transformationMatrixData = nullptr;

	Transform transform;

	Model* model = nullptr;

	Camera* camera = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_;

	Material* materialData_ = nullptr;
};

