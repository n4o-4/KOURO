#pragma once
#include "EnemyState.h"
#include "RandomGenerate.h"

class AttackState : public EnemyState
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

	int fireCount_ = 0;

	static constexpr float kChargeTime_ = 1.0f;
	float chargeTimer_ = 0.0f;
	static constexpr float kFireInterval_ = 0.5f;
	float fireIntervalTimer_ = 0.0f;

};

