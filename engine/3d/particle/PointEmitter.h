#pragma once
#include "BaseParticleEmitter.h"

namespace Kouro
{
	class PointEmitter : public BaseParticleEmitter
	{
	private:

		struct Emitter
		{
			Vector3 translate; // 位置
			uint32_t count; // 射出数

			Vector3 velocityMin;
			float pad1;

			Vector3 velocityMax;
			float pad2;

			float lifeTimeMin;
			float lifeTimeMax;
			float frequency; // 射出間隔
			float frequencyTime; // 射出間隔調整用時間

			uint32_t emit; // 射出許可
			float pad3[3];
		};

	public:

		/// \brief 初期化
		void Initialize(std::string groupName, EngineContext context) override;

		/// \brief 更新
		void Update() override;

		/// \brief エミット
		void Emit(const Matrix4x4& mat);

		/**
		* \brief  エミッターのプロパティを設定する関数
		* \param  translate       : エミッターの位置
		* \param  count           : 射出数
		* \param  velocityMin     : 射出速度の最小値
		* \param  velocityMax     : 射出速度の最大値
		* \param  lifeTimeMin     : 射出寿命の最小値
		* \param  lifeTimeMax     : 射出寿命の最大値
		* \param  frequency       : 射出間隔
		*/
		void SetEmitterProperties(Vector3 translate, uint32_t count, Vector3 velocityMin, Vector3 velocityMax, float lifeTimeMin, float lifeTimeMax, float frequency);

		WorldTransform* GetWorldTransform() { return wTransform_.get(); }

	private:

		/// \brief エミッターリソース作成関数
		void CreateEmitterResource();

		/// \brief ワールド変換用リソース作成関数
		void CreateTransformResource();

	private:

		DirectXCommon* dxCommon_ = nullptr; //!< DirectXCommonのポインタ

		Microsoft::WRL::ComPtr<ID3D12Resource> emitterResource_ = nullptr; //!< エミッター用のリソース
		Emitter* emitter_ = nullptr; //!< マップ用のポインタ

		Microsoft::WRL::ComPtr<ID3D12Resource> transformResource_ = nullptr; //!< ワールド変換用のリソース
		Kouro::Matrix4x4* transform_ = nullptr; //!< ワールド変換用のマップポインタ

		std::unique_ptr<WorldTransform> wTransform_; //!< ワールド変換クラス

		float kInterpolationStep_ = 1.0f; //!< 補間距離
	};
}
