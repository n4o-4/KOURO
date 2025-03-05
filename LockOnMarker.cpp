#include "LockOnMarker.h"

void LockOnMarker::Initialize(size_t count) {

	//Multi lock on

	multiLockOns_.clear();
	for (size_t i = 0; i < count; ++i) {
		MultiLockOn lockOn;

        // UR
        lockOn.lockOnUR = std::make_unique<Object3d>();
        lockOn.lockOnUR->Initialize(Object3dCommon::GetInstance());
        ModelManager::GetInstance()->LoadModel("lockOn/Lock_onUR.obj");
        lockOn.lockOnUR->SetModel("lockOn/Lock_onUR.obj");

        lockOn.worldTransformUR = std::make_unique<WorldTransform>();
        lockOn.worldTransformUR->Initialize();

        // UL
        lockOn.lockOnUL = std::make_unique<Object3d>();
        lockOn.lockOnUL->Initialize(Object3dCommon::GetInstance());
        ModelManager::GetInstance()->LoadModel("lockOn/Lock_onUL.obj");
        lockOn.lockOnUL->SetModel("lockOn/Lock_onUL.obj");

        lockOn.worldTransformUL = std::make_unique<WorldTransform>();
        lockOn.worldTransformUL->Initialize();

        // DR
        lockOn.lockOnDR = std::make_unique<Object3d>();
        lockOn.lockOnDR->Initialize(Object3dCommon::GetInstance());
        ModelManager::GetInstance()->LoadModel("lockOn/Lock_onDR.obj");
        lockOn.lockOnDR->SetModel("lockOn/Lock_onDR.obj");

        lockOn.worldTransformDR = std::make_unique<WorldTransform>();
        lockOn.worldTransformDR->Initialize();

        // DL
        lockOn.lockOnDL = std::make_unique<Object3d>();
        lockOn.lockOnDL->Initialize(Object3dCommon::GetInstance());
        ModelManager::GetInstance()->LoadModel("lockOn/Lock_onDL.obj");
        lockOn.lockOnDL->SetModel("lockOn/Lock_onDL.obj");

        lockOn.worldTransformDL = std::make_unique<WorldTransform>();
        lockOn.worldTransformDL->Initialize();

        lockOn.isVisible = false;
        lockOn.rotation = 0.0f; 

        multiLockOns_.push_back(std::move(lockOn));
    }

	

}

void LockOnMarker::Show(size_t index) {
    if (index < multiLockOns_.size()) {
        multiLockOns_[index].isVisible = true;
    }
}

void LockOnMarker::Hide() {
    for (auto& lockOn : multiLockOns_) {
        lockOn.isVisible = false;
    }
}

void LockOnMarker::Update() {
    for (auto& lockOn : multiLockOns_) {
        lockOn.rotation += 0.05f; 

        lockOn.worldTransformUR->UpdateMatrix();
        lockOn.worldTransformUL->UpdateMatrix();
        lockOn.worldTransformDR->UpdateMatrix();
        lockOn.worldTransformDL->UpdateMatrix();

       /* lockOn.lockOnUR->SetLocalMatrix(RotateY(lockOn.rotation));
        lockOn.lockOnUL->SetLocalMatrix(RotateY(lockOn.rotation));
        lockOn.lockOnDR->SetLocalMatrix(RotateY(lockOn.rotation));
        lockOn.lockOnDL->SetLocalMatrix(RotateY(lockOn.rotation));*/

        lockOn.lockOnUR->Update();
        lockOn.lockOnUL->Update();
        lockOn.lockOnDR->Update();
        lockOn.lockOnDL->Update();
    }
}

void LockOnMarker::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
    for (auto& lockOn : multiLockOns_) {
        if (lockOn.isVisible) {
            lockOn.lockOnUR->Draw(*lockOn.worldTransformUR, viewProjection, directionalLight, pointLight, spotLight);
            lockOn.lockOnUL->Draw(*lockOn.worldTransformUL, viewProjection, directionalLight, pointLight, spotLight);
            lockOn.lockOnDR->Draw(*lockOn.worldTransformDR, viewProjection, directionalLight, pointLight, spotLight);
            lockOn.lockOnDL->Draw(*lockOn.worldTransformDL, viewProjection, directionalLight, pointLight, spotLight);
        }
    }
}

void LockOnMarker::SetTargetPosition(size_t index, const Vector3& position) {
    if (index < multiLockOns_.size()) {
        float offset = 1.0f;

        multiLockOns_[index].worldTransformUR->transform.translate = { position.x + offset, position.y + offset, position.z };
        multiLockOns_[index].worldTransformUL->transform.translate = { position.x - offset, position.y + offset, position.z };
        multiLockOns_[index].worldTransformDR->transform.translate = { position.x + offset, position.y - offset, position.z };
        multiLockOns_[index].worldTransformDL->transform.translate = { position.x - offset, position.y - offset, position.z };
    }
}
