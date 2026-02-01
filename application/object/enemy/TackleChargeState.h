#pragma once
#include "EnemyState.h"
#include "RandomGenerate.h"

class TackleChargeState : public EnemyState
{
public:
	/**
	* \brief  開始処理
	* \param  enemy  : 処理をする敵のポインタ
	* \param  player : プレイヤー
	*/
	void OnEnter(Enemy* enemy) override;

	/**
	* \brief  更新処理
	* \param  処理をする敵のポインタ
	*/
	void Update(Enemy* enemy) override;

	/**
	* \brief  終了処理
	* \param  処理をする敵のポインタ
	*/
	void OnExit(Enemy* enemy) override;

private:

	Kouro::RandomGenerate randomGenerate;

	Kouro::WorldTransform* transform_ = nullptr; //!< 敵のワールドトランスフォーム

	Kouro::Vector3 forward_ = {}; //!< 突進方向

	Kouro::Vector3 offset_ = {}; //!< オフセット

	float timer_ = 0.0f; //!< タイマー
};

