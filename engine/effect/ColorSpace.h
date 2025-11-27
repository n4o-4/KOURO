#pragma once
#include "BaseEffect.h"

namespace ColorSpaceShader
{
	struct Material
	{
		float hue;
		float saturation;
		float value;
		float padding[1];
	};
}

// \brief ColorSpace  
// HSV（色相・彩度・明度）を操作して画面全体の色調を変化させるポストエフェクトクラス。  
// パイプライン生成とマテリアル制御を行い、色味調整やトーン補正などの効果を適用する。

class ColorSpace : public BaseEffect
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

	void CreatePipeline();


	void CreateRootSignature(Pipeline* pipeline);


	void CreatePipeLineState(Pipeline* pipeline);

	// マテリアルの生成
	void CreateMaterial();

	void DrawImGui() override;

public:

	float hue = 0.0f;
	float saturation = 0.0f;
	float value = 0.0f;	

private: // メンバ変数

	// リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

	// ポインター
	ColorSpaceShader::Material* data_ = nullptr;

};

