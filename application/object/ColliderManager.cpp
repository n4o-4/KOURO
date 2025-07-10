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
        }
    }

    for (size_t i = 0; i < count; ++i)
    {
        std::visit([](auto* collider)
			{
                //衝突状態を更新
                collider->UpdateCollisionStates();
			}, colliders_[i]);
    }
}

void ColliderManager::CheckCollision(ColliderVariant a, ColliderVariant b)
{
    uint32_t attrA, maskA, attrB, maskB;

    std::visit([&](auto* colA) {
        attrA = colA->GetCollisionAttribute();
        maskA = colA->GetCollisionMask();
        }, a);

    std::visit([&](auto* colB) {
        attrB = colB->GetCollisionAttribute();
        maskB = colB->GetCollisionMask();
        }, b);

    if ((attrA & maskB) == 0 || (attrB & maskA) == 0) {
        return;
    }

    // 型を分岐して衝突判定
    std::visit([](auto* colliderA, auto* colliderB)
        {
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

            // 衝突していたらお互いに記録
            if (isHit) {
                colliderA->AddCollision(colliderB);
                colliderB->AddCollision(colliderA);
            }

        }, a, b);
}