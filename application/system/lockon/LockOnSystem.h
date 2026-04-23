#pragma once
// standard Library
#include <memory>
#include <vector>

// engine
#include "MyMath.h"

// application
#include "Enemy.h"
#include "Reticle.h"
#include "ILockOnState.h"

/// \brief  LockOnSystem ロックオンシステム
class LockOnSystem
{
public: // 公開メンバ関数

	/**
	* \brief  初期化
	* \param  reticle ロックオン用のクロスヘア
	*/
	void Initialize(Reticle* reticle);

	/**
	* \brief  更新
	* \param  enemies ロックオン対象の敵のリスト
	*/
	void Update(std::vector<Enemy*> enemies);

private:

	Reticle* reticle_; // ロックオン用のクロスヘア

	std::unique_ptr<ILockOnState> state_; // ロックオン状態のポインタ

	std::vector<Enemy*> enemies_; // ロックオン対象の敵のリスト

	std::vector<Enemy*> lockedOnEnemies_; // ロックオンされている敵のリスト
};