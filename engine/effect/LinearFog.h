#pragma once
#include "BaseEffect.h"

namespace LinearFogShader
{
	struct Material
	{
		Matrix4x4 projectionInverse;
	};

}

// \brief LinearFog  
// カメラとの距離に応じて線形に霧をかけるポストエフェクトクラス。  
// 遠景を淡くぼかし、空間の奥行きや大気感を表現する。

class LinearFog : public BaseEffect
{
public:

	/**
	* \brief	初期化
	* \param	dxCommon DirectXCommonのポインタ
	* \param	srvManager SrvManagerのポインタ
	*/
	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager) override;

	/// \brief  更新
	void Update() override;

	/**
	* \brief  描画
	* \param  renderTargetIndex レンダーターゲットのインデックス
	* \param  renderResourceIndex レンダーリソースのインデックス
	*/
	void Draw(uint32_t renderTargetIndex, uint32_t renderResourceIndex) override;

	/**
	* \brief  カメラマネージャーを設定する
	* \param  cameraManager カメラマネージャーのポインタ
	*/
	void SetCameraManager(CameraManager* cameraManager) override { cameraManager_ = cameraManager; }

	/// \brief  リセット
	void Reset() override { resource_.Reset(); }

private: // 非公開メンバ関数

	/// \brief  CreatePipeline パイプラインの作成
	void CreatePipeline();

	/**
	* \brief  CreateRootSignature ルートシグネチャの作成
	* \param  pipeline パイプライン構造体のポインタ
	*/
	void CreateRootSignature(Pipeline* pipeline);

	/**
	* \brief  CreatePipeLineState パイプラインステートの作成
	* \param  pipeline パイプライン構造体のポインタ
	*/
	void CreatePipeLineState(Pipeline* pipeline);

	/// \brief  CreateMaterial マテリアルの作成
	void CreateMaterial();

	/// \brief  DrawImGui ImGui描画
	void DrawImGui() override;

private: // メンバ変数

	// 
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

	// 
	LinearFogShader::Material* data_ = nullptr;

	//
	CameraManager* cameraManager_ = nullptr;
};

