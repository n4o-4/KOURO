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

