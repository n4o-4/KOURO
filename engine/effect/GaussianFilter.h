#pragma once
#include "BaseEffect.h"

// \brief GaussianFilter  
// ガウス関数に基づいて画像を滑らかにぼかすポストエフェクトクラス。  
// ボックスフィルターより自然なブラー効果を実現し、光のにじみや被写界深度表現などに使用される。

class GaussianFilter : public BaseEffect
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

	/// \brief  DrawImGui ImGui描画
	void DrawImGui() override;
};

