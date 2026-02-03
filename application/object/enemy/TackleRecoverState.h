#pragma once
#include "EnemyState.h"
class TackleRecoverState : public EnemyState
{
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

	float timer_ = 0.0f;

	float duration_ = 2.0f; // 回復時間

	Kouro::Vector3 startPos_ = {};
};

