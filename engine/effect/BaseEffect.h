#pragma once
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "CameraManager.h"

#include "Vectors.h"

struct Pipeline
{
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
};

// \brief BaseEffect ポストエフェクト基底クラス

class BaseEffect
{
public:

	/// \brief デストラクタ
	virtual ~BaseEffect() = default;

	/**
	* \brief  初期化
	* \param  dxCommon DirectXCommonのポインタ
	* \param  srvManager SrvManagerのポインタ
	*/
	virtual void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager);

	/// \brief  更新
	virtual void Update() = 0;

	/**
	* \brief  描画
	* \param  renderTargetIndex レンダーターゲットのインデックス
	* \param  renderResourceIndex レンダーリソースのインデックス
	*/
	virtual void Draw(uint32_t renderTargetIndex, uint32_t renderResourceIndex) = 0;

	/**
	* \brief  カメラマネージャーのセット
	* \param  cameraManager カメラマネージャーのポインタ
	*/
	virtual void SetCameraManager(CameraManager* cameraManager);

	/// \brief  リセット
	virtual void Reset();

private:

	/// \brief  DrawImGui ImGui描画
	virtual void DrawImGui() = 0;  // 追加

protected:

	DirectXCommon* dxCommon_ = nullptr;

	SrvManager* srvManager_ = nullptr;

	std::unique_ptr<Pipeline> pipeline_ = nullptr;
};

