#pragma once
#include "EnemyState.h"

// \brief ApproachState 接近状態クラス

class ApproachState : public EnemyState
{
	public:
		/**
		* \brief  開始処理
		* \param  enemy  : 処理をする敵のポインタ
		* \param  player : プレイヤー
		*/
		void Enter(Enemy* enemy, const WorldTransform* worldTransform) override;

		/**
		* \brief  更新処理
		* \param  処理をする敵のポインタ
		*/
		void Update(Enemy* enemy) override;

		/**
		* \brief  終了処理
		* \param  処理をする敵のポインタ
		*/
		void Exit(Enemy* enemy) override;


		float ToRadians(float degree)
		{
			constexpr float kPi = 3.1415926535f;
			return degree * (kPi / 180.0f);
		}
private: // 非公開メンバ変数

	const WorldTransform* parent_ = nullptr;

	float angle_ = 0.0f;

	Vector3 kGoalOffset_ = {};

private: // 非公開メンバ定数

	// 定数（演出調整用）
	static constexpr float kApproachRadius = 100.0f;     // 弧の半径
	static constexpr float kApproachSpeed = 90.0f;     // 弧を描く角速度（度/秒）
	static constexpr float kApproachHeight = 5.0f;      // 上下方向のうねり
	static constexpr float kEndAngle = 180.0f;    // 何度まで動かすか（180で背後→前方）
};