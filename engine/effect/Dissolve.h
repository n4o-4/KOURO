#pragma once
#include "BaseEffect.h"

namespace DissolveShader
{
	struct Material
	{
		Vector3 edgeColor;
		float threshold;
		float thresholdWidth;
		float mask;
		float padding[2];
	};
}

// \brief Dissolve  
// マスクテクスチャを利用してオブジェクトを徐々に消失させるディゾルブ効果を実現するポストエフェクトクラス。  
// エッジカラーや閾値を調整し、溶けるようなフェードアウト演出を行う。

class Dissolve : public BaseEffect
{
public:

	// 初期化
	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager) override;

	// 更新
	void Update() override;

	// 描画
	void Draw(uint32_t renderTargetIndex, uint32_t renderResourceIndex) override;

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

public:

	Vector3 edgeColor;

	float threshold;

	float thresholdWidth;

	float mask;

private: // メンバ変数

	// 
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

	// 
	DissolveShader::Material* data_ = nullptr;

};

