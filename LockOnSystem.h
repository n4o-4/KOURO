#pragma once

// engine
#include "MyMath.h"

// application
#include "Enemy.h"
#include "Reticle.h"


/// \brief  LockOnSystem ロックオンシステム
class LockOnSystem
{
public:

	void Initialize();

	void Update();

private:

	Reticle* reticle_; // ロックオン用のクロスヘア
};