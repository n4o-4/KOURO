#pragma once
#include "BaseEffect.h"

// \brief Grayscale  
// シーン全体をモノクロ化するポストエフェクトクラス。  
// カラー画像を輝度値に変換し、映像の演出やトーン調整に利用される。

class Grayscale : public BaseEffect
{
public:

	// 初期化
	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager) override;

	// 更新
	void Update() override;

	// 描画
	void Draw(uint32_t renderTargetIndex, uint32_t renderResourceIndex) override;

private:

	void CreatePipeline();


	void CreateRootSignature(Pipeline* pipeline);
	

	void CreatePipeLineState(Pipeline* pipeline);

	void DrawImGui() override;
};

