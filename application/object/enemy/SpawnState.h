#pragma once
#include "EnemyState.h"
#include "Vector3.h"

class Enemy; // 前方宣言

class SpawnState : public EnemyState
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
	Kouro::Vector3 kInitialScale_ = { 0.0f, 0.0f, 0.0f }; // 初期スケール
	Kouro::Vector3 kBaseScale_ = { 1.0f, 1.0f, 1.0f }; // 基本スケール
	const float kSpawnDuration_ = 0.6f; // 出現時間
	float timer_ = 0.0f; // 出現タイマー
	const float kOvershoot_ = 5.0f; // オーバーシュートの量
};

