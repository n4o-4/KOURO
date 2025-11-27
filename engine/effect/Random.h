#pragma once
#include <random>

#include "BaseEffect.h"

namespace RandomShader
{
	struct Material
	{
		float time;
		float padding[3];
	};
}

// \brief Random  
// 画面全体にランダムなノイズや揺らぎ効果を適用するポストエフェクトクラス。  
// ノイズ演出やダメージ表現など、映像にランダム性を加える際に使用される。

class Random : public BaseEffect
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

private: // メンバ変数

	// 
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

	// 
	RandomShader::Material* data_ = nullptr;

	std::mt19937 randomEngine;
};

