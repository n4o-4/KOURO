#pragma once
#include "EnemyState.h"
#include "RandomGenerate.h"

class RandomMoveState : public EnemyState
{
public:

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

	float stateTimer_ = 0.0f;

	Kouro::Vector3 velocity_ = {};
};

