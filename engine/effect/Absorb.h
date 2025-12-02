#pragma once
#include "BaseEffect.h"

namespace AbsorbShader
{
	struct Material
	{
		Vector4 glowColor; // 発光カラー（例: float3(1.0, 1.0, 1.0)）
        Vector2 center; // 吸い込み中心（例: float2(0.5, 0.5)）
		float time; // 0.0 → 1.0
		float intensity; // 吸い込みの強さ

		float glowPower; // 発光の強さ
		float padding[3];
	};
}

class Absorb : public BaseEffect
{
public:

	/**
	* \brief  Initialize 初期化
	* \param  dxCommon DirectX共通
	* \param  srvManager SRVマネージャー
	*/
	void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager) override;

	/**
	* \brief  Update 更新
	*/
	void Update() override;

	/**
	* \brief  Draw 描画
	* \param  renderTargetIndex レンダーターゲットのインデックス
	* \param  renderResourceIndex レンダーリソースのインデックス
	*/
	void Draw(uint32_t renderTargetIndex, uint32_t renderResourceIndex) override;

	/**
	* \brief  GetMaterialData
	* \return マテリアルデータのポインター
	*/
	const AbsorbShader::Material& GetMaterialData() const { return *data_; }

	/**
	* \brief  SetMaterialData
	* \param  data マテリアルデータ
	*/
	void SetMaterialData(const AbsorbShader::Material& data) { *data_ = data; }

private:

	/**
	*/
	void CreatePipeline();


	void CreateRootSignature(Pipeline* pipeline);


	void CreatePipeLineState(Pipeline* pipeline);


	// マテリアルの生成
	void CreateMaterial();

	void DrawImGui() override;

private:

	// リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

	// ポインター
	AbsorbShader::Material* data_ = nullptr;
};