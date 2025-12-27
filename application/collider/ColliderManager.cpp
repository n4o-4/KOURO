#include "ColliderManager.h"

void ColliderManager::Update()
{
    // 生きていないコライダーをリストから削除する
    std::erase_if(colliders_, [](const ColliderVariant& collider) {
        return std::visit([](auto& ptr) {
            return ptr && !ptr->GetIsAlive();
            }, collider);
    });

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
                if (collider) { // nullptr チェック（shared_ptrが空でないか）
                    collider->UpdateCollisionStates();
                }
            }, colliders_[i]);
    }
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

            using A = std::decay_t<decltype(*colliderA)>;
            using B = std::decay_t<decltype(*colliderB)>;

            bool isHit = false;

            if constexpr (std::is_same_v<A, AABBCollider> && std::is_same_v<B, AABBCollider>) {
                isHit = IsCollision(colliderA->GetAABB(), colliderB->GetAABB());
            }
            else if constexpr (std::is_same_v<A, SphereCollider> && std::is_same_v<B, SphereCollider>) {
                isHit = IsCollision(colliderA->GetSphere(), colliderB->GetSphere());
            }
            else if constexpr (std::is_same_v<A, OBBCollider> && std::is_same_v<B, OBBCollider>) {
                isHit = IsCollision(colliderA->GetOBB(), colliderB->GetOBB());
            }
            else if constexpr (std::is_same_v<A, AABBCollider> && std::is_same_v<B, SphereCollider>) {
                isHit = IsCollision(colliderA->GetAABB(), colliderB->GetSphere());
            }
            else if constexpr (std::is_same_v<A, SphereCollider> && std::is_same_v<B, AABBCollider>) {
                isHit = IsCollision(colliderA->GetSphere(), colliderB->GetAABB());
            }
            else if constexpr (std::is_same_v<A, SphereCollider> && std::is_same_v<B, OBBCollider>) {
                isHit = IsCollision(colliderA->GetSphere(), colliderB->GetOBB());
            }
            else if constexpr (std::is_same_v<A, OBBCollider> && std::is_same_v<B, SphereCollider>) {
                isHit = IsCollision(colliderA->GetOBB(), colliderB->GetSphere());
            }
            else if constexpr (std::is_same_v<A, AABBCollider> && std::is_same_v<B, OBBCollider>) {
                isHit = IsCollision(colliderA->GetAABB(), colliderB->GetOBB());
            }
            else if constexpr (std::is_same_v<A, OBBCollider> && std::is_same_v<B, AABBCollider>) {
                isHit = IsCollision(colliderA->GetOBB(), colliderB->GetAABB());
            }

            if (isHit) {
                colliderA->AddCollision(colliderB.get());
                colliderB->AddCollision(colliderA.get());
            }

        }, a, b);
}