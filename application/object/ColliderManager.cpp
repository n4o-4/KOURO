#include "ColliderManager.h"

void ColliderManager::Update()
{
    // 無効な（消えた）コライダーをリストから削除
    colliders_.erase(
        std::remove_if(
            colliders_.begin(),
            colliders_.end(),
            [](BaseCollider* collider) {
                // nullptr 
                return collider == nullptr;
            }
        ),
        colliders_.end()
    );

    //// コライダーの更新処理
    //for (BaseCollider* collider : colliders_)
    //{
    //    collider->Update();
    //}

    ScanColliders();
}

void ColliderManager::ScanColliders()
{
	int checkCount = 0;

    // コライダー同士の当たり判定
    const size_t count = colliders_.size();
    for (size_t i = 0; i < count; ++i)
    {
        // i番目のコライダーを取得
        BaseCollider* a = colliders_[i];

        for (size_t j = i + 1; j < count; ++j)
        {
            // j番目のコライダーを取得
            BaseCollider* b = colliders_[j];

            CheckCollision(a, b);

            ++checkCount;
        }
    }

#ifdef _DEBUG

	ImGui::Begin("Collider Manager");

	ImGui::Text("Collider checkCount: %zu", checkCount);

	ImGui::Text("Collider count: %zu", count);

    ImGui::End();

#endif

    for (size_t i = 0; i < count; ++i)
    {
		// i番目のコライダーの衝突状態を更新
        colliders_[i]->UpdateCollisionStates();
    }
}

void ColliderManager::CheckCollision(BaseCollider* a, BaseCollider* b)
{
    // コリジョン属性とマスクのチェック
    if ((a->GetCollisionAttribute() & b->GetCollisionMask()) != 0 &&
        (b->GetCollisionAttribute() & a->GetCollisionMask()) != 0)
    {
        // Visitorパターンによる衝突判定
        if (a->CheckCollision(b)) {
            // 衝突している場合の処理（例：コリジョンリストへの追加など）
            a->AddCollision(b);
            b->AddCollision(a);
        }
    }
}