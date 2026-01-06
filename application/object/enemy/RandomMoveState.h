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

private:

	RandomGenerate randomGenerate;

	float stateTimer_ = 0.0f;

	Vector3 velocity_ = {};
};

