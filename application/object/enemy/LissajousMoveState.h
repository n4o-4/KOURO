#pragma once
#include "EnemyState.h"
class LissajousMoveState : public EnemyState
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

	float time_ = 0.0f;

	Kouro::WorldTransform* transform_ = nullptr;

	Kouro::Vector3 offset_ = {};

	static constexpr float kWidthAmp_ = 4.0f;

	static constexpr float kHeightAmp_ = 8.0f;
};

