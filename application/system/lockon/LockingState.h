#pragma once
// application
#include "ILockOnState.h"
#include "LockOnSystem.h"

// ロックオン状態の一つである「ロック中」の状態を表すクラス
class LockingState : public ILockOnState
{
	public:

	/// \brief  ロックオン状態の開始処理
	virtual void OnEnter(Kouro::Input* input = Kouro::Input::GetInstance()) override;

	/// \brief  ロックオン状態の更新
	virtual void Update(std::vector<Enemy*> enemies) override;

	/// \brief  ロックオン状態の終了処理
	virtual void OnExit() override;
};

