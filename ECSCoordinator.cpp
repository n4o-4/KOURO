#include "ECSCoordinator.h"

template <typename T>

void ECSCoordinator::AddComponent(Entity e, T component)
{
    componentManager_->AddComponent<T>(e, component);
    auto signature = entityManager_->GetSignature(e);
    signature.set(componentManager_->GetComponentType<T>(), true);
    entityManager_->SetSignature(e, signature);
    systemManager_->EntitySignatureChanged(e, signature);
}