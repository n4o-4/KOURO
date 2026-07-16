#include "ColliderManager.h"

void ColliderManager::Update()
{
	CleanUpColliders();

    ScanColliders();
}

void ColliderManager::AddCollider(BaseCollider* collider)
{
    if (auto* aabb = dynamic_cast<AABBCollider*>(collider))
    {
        colliders_.emplace_back(aabb);
    }
    else if (auto* sphere = dynamic_cast<SphereCollider*>(collider))
    {
        colliders_.emplace_back(sphere);
    }
    else if (auto* obb = dynamic_cast<OBBCollider*>(collider))
    {
        colliders_.emplace_back(obb);
    }

}

void ColliderManager::CleanUpColliders()
{
    // 他のコライダーの衝突リストから削除
    for (const auto& collider : colliders_)
    {
        BaseCollider* deadCollider = std::visit([](auto* ptr) -> BaseCollider*
            {
                return ptr;
            }, collider);

        if (!deadCollider || deadCollider->GetIsAlive())
        {
            continue;
        }

        // 他の全コライダーから削除
        for (const auto& other : colliders_)
        {
            BaseCollider* otherCollider = std::visit([](auto* ptr) -> BaseCollider*
                {
                    return ptr;
                }, other);

            if (!otherCollider || otherCollider == deadCollider)
            {
                continue;
            }

            otherCollider->RemoveCollision(deadCollider);
        }
    }

    // 生きていないコライダーを削除
    std::erase_if(colliders_, [](const ColliderVariant& collider)
        {
            return std::visit([](auto* ptr)
                {
                    return ptr && !ptr->GetIsAlive();
                }, collider);
        });
}

void ColliderManager::ScanColliders()
{
	int checkCount = 0;

    // コライダー同士の当たり判定
    const size_t count = colliders_.size();
    for (size_t i = 0; i < count; ++i)
    {
        // i番目のコライダーを取得
        ColliderVariant a = colliders_[i];

        for (size_t j = i + 1; j < count; ++j)
        {
            // j番目のコライダーを取得
            ColliderVariant b = colliders_[j];

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
        std::visit([](auto& collider)
            {
                if (collider)
                {
                    collider->UpdateCollisionStates();
                }
            }, colliders_[i]);
    }

	CleanUpColliders();
}

void ColliderManager::CheckCollision(ColliderVariant a, ColliderVariant b)
{
    uint32_t attrA = 0, maskA = 0, attrB = 0, maskB = 0;

    std::visit([&](auto& colA) {
        if (colA) { // nullptr チェック
            attrA = colA->GetCollisionAttribute();
            maskA = colA->GetCollisionMask();
        }
        }, a);

    std::visit([&](auto& colB) {
        if (colB) {
            attrB = colB->GetCollisionAttribute();
            maskB = colB->GetCollisionMask();
        }
        }, b);

    if ((attrA & maskB) == 0 || (attrB & maskA) == 0) {
        return;
    }

    // 型を分岐して衝突判定
    std::visit([](auto& colliderA, auto& colliderB)
        {
            if (!colliderA || !colliderB) return;

            // BaseCollider* に変換
            BaseCollider* baseA = colliderA;
            BaseCollider* baseB = colliderB;

           /* char buf[128];
            sprintf_s(buf,
                "baseA=%p baseB=%p\n",
                baseA,
                baseB);
            OutputDebugStringA(buf);

            OutputDebugStringA(typeid(*baseA).name());
            OutputDebugStringA("\n");*/

            // ダブルディスパッチで衝突判定
            if (baseA->CheckCollision(baseB))
            {
                baseA->AddCollision(baseB);
                baseB->AddCollision(baseA);
            }

        }, a, b);
}