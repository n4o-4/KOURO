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

	// 初期化
	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager) override;

	// 更新
	void Update() override;

	// 描画
	void Draw(uint32_t renderTargetIndex, uint32_t renderResourceIndex) override;

	// カメラマネージャの設定
	void SetCameraManager(CameraManager* cameraManager) override { cameraManager_ = cameraManager; }

	// リソースの解放
	void Reset() override { resource_.Reset(); }

private:

	// パイプラインの生成
	void CreatePipeline();

	// ルートシグネチャの生成と設定
	void CreateRootSignature(Pipeline* pipeline);

	// パイプラインステートの生成と設定
	void CreatePipeLineState(Pipeline* pipeline);

	// マテリアルの生成
	void CreateMaterial();

	void DrawImGui() override;

private: // メンバ変数

	// 
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

	// 
	LinearFogShader::Material* data_ = nullptr;

	//
	CameraManager* cameraManager_ = nullptr;
};

