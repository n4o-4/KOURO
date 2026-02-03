#include "TackleRecoverState.h"
#include "Enemy.h"

#include "LissajousMoveState.h"
#include "RandomMoveState.h"

void TackleRecoverState::OnEnter(Enemy* enemy)
{
    timer_ = 0.0f;

    // Dummy の開始位置を保存
    startPos_ = enemy->GetDummyPosition();

    // 念のため速度は止める
    enemy->SetDummyVelocity({ 0.0f, 0.0f, 0.0f });
}

void TackleRecoverState::Update(Enemy* enemy)
{
    timer_ += 1.0f / 60.0f;

    float t = timer_ / duration_;
    t = std::clamp(t, 0.0f, 1.0f);

    Kouro::Vector3 enemyPos = enemy->GetWorldPosition();

    // 線形補間で戻す
    Kouro::Vector3 newPos = Kouro::Lerp(enemyPos, startPos_, t);

    enemy->SetDummyPosition(newPos);

    // 時間終了
    if (t >= 1.0f)
    {
        // 位置を完全一致
        enemy->SetDummyPosition(enemyPos);

        // Dummy を消す
        enemy->SetDrawDummy(false);

        // 次の State へ
        enemy->ChangeState(std::make_unique<RandomMoveState>());
    }
}

void TackleRecoverState::OnExit(Enemy* enemy)
{
	enemy->SetDummyVelocity({ 0.0f, 0.0f, 0.0f });
	enemy->SetDrawDummy(false);
}
