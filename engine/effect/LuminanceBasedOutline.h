#pragma once
#include "BaseEffect.h"

namespace LuminanceOutline
{
	struct Material
	{
		float edgeStrength;
		float padding[3];
	};
}

// \brief LuminanceBasedOutline  
// 輝度差をもとに物体の輪郭を抽出するポストエフェクトクラス。  
// 明暗の境界を強調し、セル画風やスケッチ風の表現を実現する。

class LuminanceBasedOutline : public BaseEffect
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

	// メモリリーク発生
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

	// 
	LuminanceOutline::Material* data_ = nullptr;

};

