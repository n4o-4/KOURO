#pragma once
#include "BaseEffect.h"

namespace Radial
{
	struct Material
	{
		int numSamples;
		Vector2 center;
		float blurWidth;
	};
}

// \brief RadialBlur  
// 画面の中心から放射状に広がるブラー効果を適用するポストエフェクトクラス。  
// 爆発や加速などの演出で、動きの勢いや焦点効果を強調する。

class RadialBlur : public BaseEffect
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
	Radial::Material* data_ = nullptr;
};

