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

	/**
	* \brief  初期化
	* \param  dxCommon DirectXCommonのポインタ
	* \param  srvManager SrvManagerのポインタ
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
	* \brief  MaterialDataを取得する
	* \return Matrialの値
	*/
	const Radial::Material GetMaterial() { return *data_; }

	/**
	* \brief  Materialの値を設定する
	* \param  material : マテリアル
	*/
	void SetMaterialData(Radial::Material material) { data_->blurWidth = material.blurWidth, data_->numSamples = material.numSamples; }

	// リソースの解放
	void Reset() override { resource_.Reset(); }

private:

	// パイプラインの生成
	void CreatePipeline();

	/**
	* \brief  CreateRootSignature ルートシグネチャの生成
	* \param  pipeline パイプライン構造体のポインタ
	*/
	void CreateRootSignature(Pipeline* pipeline);

	/**
	* \brief  CreatePipeLineState パイプラインステートの生成
	* \param  pipeline パイプライン構造体のポインタ
	*/
	void CreatePipeLineState(Pipeline* pipeline);

	// マテリアルの生成
	void CreateMaterial();

	/// \brief  DrawImGui ImGui描画
	void DrawImGui() override;

private: // メンバ変数

	// 
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

	// 
	Radial::Material* data_ = nullptr;
};

