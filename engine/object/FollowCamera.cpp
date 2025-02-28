#include "FollowCamera.h"
#include "Player.h"

void FollowCamera::Initialize() {
	// 初期化処理（特になし）
}

void FollowCamera::Update(Player* player) {
	if (!player) return;

	// プレイヤーの現在位置を取得
	Vector3 playerPos = player->GetPosition();

	// 目標カメラ位置（プレイヤーの背後）
	Vector3 targetCameraPos = playerPos + offset_;

	// 現在のカメラ位置を取得
	Vector3 currentCameraPos = Camera::GetInstance()->GetTranslate();

	// 線形補間（Lerp）でスムーズに追従
	float followSpeed = 0.01f; // 追従速度（0.0fに近いほど遅い、1.0fだと即座に移動）
	Vector3 newCameraPos = Lerp(currentCameraPos, targetCameraPos, followSpeed);

	// Camera の位置を設定
	Camera::GetInstance()->SetTranslate(newCameraPos);
}
