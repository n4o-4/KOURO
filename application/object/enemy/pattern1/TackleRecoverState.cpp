#include "TackleRecoverState.h"
#include "Enemy.h"

#include "LissajousMoveState.h"
#include "RandomMoveState.h"

void TackleRecoverState::OnEnter(Enemy* enemy)
{
    timer_ = 0.0f;
}

void TackleRecoverState::Update(Enemy* enemy)
{
    timer_ += 1.0f / 60.0f;

    float t = timer_ / duration_;
    t = std::clamp(t, 0.0f, 1.0f);

    Kouro::Vector3 enemyPos = enemy->GetWorldPosition();

    // 線形補間で戻す
    Kouro::Vector3 newPos = Kouro::Lerp(enemyPos, startPos_, t);

    // 時間終了
    if (t >= 1.0f)
    {
        // 次の State へ
        enemy->ChangeState(std::make_unique<RandomMoveState>());
    }
}

void TackleRecoverState::OnExit(Enemy* enemy)
{
}
