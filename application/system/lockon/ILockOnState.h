#pragma once
// standard Library
#include <memory>

//application
#include "Enemy.h"

class ILockOnState
{
public: // 公開メンバ関数

	/// \brief  デストラクタ
	virtual ~ILockOnState() = default;

	/// \brief  ロックオン状態の開始処理
	virtual void OnEnter(Kouro::Input* input = Kouro::Input::GetInstance()) = 0;

	/// \brief  ロックオン状態の更新
	virtual void Update(std::vector<Enemy*> enemies) = 0;

	/// \brief  ロックオン状態の終了処理
	virtual void OnExit() = 0;

private: // 非公開メンバ関数



};

