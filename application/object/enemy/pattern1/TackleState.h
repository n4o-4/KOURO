#pragma once
#include "EnemyState.h"

class BaseCharacter;

class TackleState : public EnemyState
{
public:
	/**
	* \brief  開始処理
	* \param  enemy  : 処理をする敵のポインタ
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

	Kouro::Vector3 direction_;   // 突進方向
	float speed_ = 30.0f; // 突進速度
	float timer_ = 0.0f;
	float duration_ = 3.0f; // 突進時間
};

