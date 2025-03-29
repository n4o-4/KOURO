/*********************************************************************
 * \file   Hud.cpp
 * \brief
 *
 * \author Harukichimaru
 * \date   March 2025
 * \note
 *********************************************************************/
#include "Hud.h"

///=============================================================================
///						初期化
void Hud::Initialize(FollowCamera *followCamera, Player *player, LockOn *lockOnSystem) {
	// カメラのポインタをセット
	followCamera_ = followCamera;
	// プレイヤーのポインタをセット
	player_ = player;
	// ロックオンシステムのポインタをセット
	lockOnSystem_ = lockOnSystem;
	// ラインマネージャをセット
	lineManager_ = LineManager::GetInstance();

	// 初期状態では戦闘モードをアクティブにする
	isCombatMode_ = true;
}

///=============================================================================
///						更新
void Hud::Update() {
	// 戦闘モードでない場合は更新しない
	if(!isCombatMode_) {
		return;
	}

	// ロックオン表示の回転を更新
	lockOnRotation_ += lockOnRotationSpeed_;
	if(lockOnRotation_ > kTwoPi) {
		lockOnRotation_ -= kTwoPi;
	}
}

///=============================================================================
///						描画
void Hud::Draw(ViewProjection viewProjection) {
	// 戦闘モードでない場合は描画しない
	if(!isCombatMode_) {
		return;
	}

	// 照準の描画
	DrawCrosshair(viewProjection);

	// ロックオンの描画
	DrawLockOn(viewProjection);
}

///=============================================================================
///						戦闘モードをアクティブにする
void Hud::CombatModeActive() {
	isCombatMode_ = true;
}
///						戦闘モードを非アクティブにする
void Hud::CombatModeDeactive() {
	isCombatMode_ = false;
}

///=============================================================================
///						照準の描画
///=============================================================================
///						照準の描画
void Hud::DrawCrosshair(ViewProjection viewProjection) {
    // プレイヤーの位置を取得
    Vector3 playerPos = player_->GetPosition();

    // カメラの前方向ベクトルを取得
    Vector3 cameraForward = followCamera_->GetForwardDirection();

    // 照準の位置を計算（プレイヤーの少し前方）
    Vector3 crosshairPos = {
        playerPos.x + cameraForward.x * crosshairDistance_,
        playerPos.y + cameraForward.y * crosshairDistance_,
        playerPos.z + cameraForward.z * crosshairDistance_
    };

    // 上下左右の線の位置計算用に、カメラの右方向と上方向を取得
    Vector3 rightDir = followCamera_->GetRightDirection();
    Vector3 upDir = followCamera_->GetUpDirection();

    // 中心点
    Vector3 center = crosshairPos;

    // 右線
    Vector3 rightStart = {
        center.x + rightDir.x * crosshairGap_,
        center.y + rightDir.y * crosshairGap_,
        center.z + rightDir.z * crosshairGap_
    };

    Vector3 rightEnd = {
        rightStart.x + rightDir.x * crosshairSize_,
        rightStart.y + rightDir.y * crosshairSize_,
        rightStart.z + rightDir.z * crosshairSize_
    };

    lineManager_->DrawLine(rightStart, rightEnd, crosshairColor_);

    // 左線
    Vector3 leftStart = {
        center.x - rightDir.x * crosshairGap_,
        center.y - rightDir.y * crosshairGap_,
        center.z - rightDir.z * crosshairGap_
    };

    Vector3 leftEnd = {
        leftStart.x - rightDir.x * crosshairSize_,
        leftStart.y - rightDir.y * crosshairSize_,
        leftStart.z - rightDir.z * crosshairSize_
    };

    lineManager_->DrawLine(leftStart, leftEnd, crosshairColor_);

    // 上線
    Vector3 upStart = {
        center.x + upDir.x * crosshairGap_,
        center.y + upDir.y * crosshairGap_,
        center.z + upDir.z * crosshairGap_
    };

    Vector3 upEnd = {
        upStart.x + upDir.x * crosshairSize_,
        upStart.y + upDir.y * crosshairSize_,
        upStart.z + upDir.z * crosshairSize_
    };

    lineManager_->DrawLine(upStart, upEnd, crosshairColor_);

    // 下線
    Vector3 downStart = {
        center.x - upDir.x * crosshairGap_,
        center.y - upDir.y * crosshairGap_,
        center.z - upDir.z * crosshairGap_
    };

    Vector3 downEnd = {
        downStart.x - upDir.x * crosshairSize_,
        downStart.y - upDir.y * crosshairSize_,
        downStart.z - upDir.z * crosshairSize_
    };

    lineManager_->DrawLine(downStart, downEnd, crosshairColor_);

    // 中央の小さい円（カメラに正対するように変更）
    DrawFacingCircle(center, crosshairCenterRadius_, crosshairColor_, crosshairCircleSegments_, cameraForward);
}

void Hud::DrawLockOn(ViewProjection viewProjection) {
	// ロックオンシステムがない場合は何もしない
	if(!lockOnSystem_) {
		return;
	}

	// ロックオンしている敵の数を取得
	const auto &lockedEnemies = lockOnSystem_->GetLockedEnemies();

	// カメラの方向ベクトルを取得
	Vector3 cameraForward = followCamera_->GetForwardDirection();
	Vector3 cameraRight = followCamera_->GetRightDirection();
	Vector3 cameraUp = followCamera_->GetUpDirection();

	// 各ロックオンターゲットに対して描画
	for(auto *enemy : lockedEnemies) {
		if(!enemy) continue;

		// 敵の位置を取得
		Vector3 enemyPos = enemy->GetPosition();

		// ロックオンレベルに応じた表示
		LockOn::LockLevel lockLevel = lockOnSystem_->GetLockLevel(enemy);

		Vector4 markerColor = lockOnColor_;
		float markerSize = lockOnSize_;

		// ロックオンレベルによって色や表示を変更
		switch(lockLevel) {
		case LockOn::LockLevel::PreciseLock:
		{
			// 精密ロックオン
			markerColor = preciseLockonColor_;
			markerSize = lockOnSize_ * preciseLockonSizeRatio_;

			// カメラ方向に対して回転する十字を描画
			float cosRot = cosf(lockOnRotation_);
			float sinRot = sinf(lockOnRotation_);

			// カメラの向きに合わせた十字線の計算
			Vector3 dir1 = {
				cameraRight.x * cosRot + cameraUp.x * sinRot,
				cameraRight.y * cosRot + cameraUp.y * sinRot,
				cameraRight.z * cosRot + cameraUp.z * sinRot
			};

			Vector3 dir2 = {
				cameraRight.x * -sinRot + cameraUp.x * cosRot,
				cameraRight.y * -sinRot + cameraUp.y * cosRot,
				cameraRight.z * -sinRot + cameraUp.z * cosRot
			};

			// 回転する斜めの十字
			lineManager_->DrawLine(
				{ enemyPos.x + dir1.x * markerSize, enemyPos.y + dir1.y * markerSize, enemyPos.z + dir1.z * markerSize },
				{ enemyPos.x - dir1.x * markerSize, enemyPos.y - dir1.y * markerSize, enemyPos.z - dir1.z * markerSize },
				markerColor
			);

			lineManager_->DrawLine(
				{ enemyPos.x + dir2.x * markerSize, enemyPos.y + dir2.y * markerSize, enemyPos.z + dir2.z * markerSize },
				{ enemyPos.x - dir2.x * markerSize, enemyPos.y - dir2.y * markerSize, enemyPos.z - dir2.z * markerSize },
				markerColor
			);

			// カメラの向きに合わせた円の描画（カメラに正対する円）
			DrawFacingCircle(enemyPos, markerSize * preciseLockonInnerRatio_, markerColor, crosshairCircleSegments_, cameraForward);
			break;
		}
		case LockOn::LockLevel::SimpleLock:
			// 簡易ロックオン
			markerColor = simpleLockonColor_;

			// カメラに向いた正方形を描画
			DrawFacingSquare(enemyPos, markerSize * simpleLockonSizeRatio_, markerColor, cameraRight, cameraUp);
			break;

		case LockOn::LockLevel::None:
		default:
			// ロックオンなし/デフォルト
			markerColor = noLockonColor_;

			// カメラの向きに合わせた円の描画
			DrawFacingCircle(enemyPos, markerSize * noLockonSizeRatio_, markerColor, crosshairCircleSegments_, cameraForward);
			break;
		}

		// ロックオン進行状況インジケーター（PreciseLockへの進行度）
		LockOn::LockedEnemyInfo *info = lockOnSystem_->GetLockedEnemyInfo(enemy);
		if(info && lockLevel == LockOn::LockLevel::SimpleLock) {
			// ロックオン進行度を計算（0.0～1.0）
			float lockProgress = info->lockTimer / lockOnSystem_->GetTimeToPreciseLock();
			lockProgress = std::min(lockProgress, 1.0f);

			// カメラに正対する進行度円弧を描画
			DrawFacingProgressArc(
				enemyPos,
				markerSize * progressRadiusRatio_,
				progressStartAngle_,
				progressStartAngle_ + ( kTwoPi * lockProgress ),
				progressColor_,
				progressArcSegments_,
				cameraRight,
				cameraUp
			);
		}
	}
}

// カメラに正対する円を描画する補助関数
void Hud::DrawFacingCircle(const Vector3 &center, float radius, const Vector4 &color, int segments, const Vector3 &cameraForward) {
	// カメラの前方向に垂直な平面上に円を描画
	Vector3 up = { 0.0f, 1.0f, 0.0f };
	// カメラの前方向とほぼ平行な場合は別の基準軸を使用
	if(fabsf(cameraForward.y) > 0.99f) {
		up = { 0.0f, 0.0f, 1.0f };
	}

	Vector3 right = Cross(up, cameraForward);
	Normalize(right);

	up = Cross(cameraForward, right);
	Normalize(up);

	// 円を描く
	for(int i = 0; i < segments; i++) {
		float angle1 = i * kTwoPi / segments;
		float angle2 = ( i + 1 ) * kTwoPi / segments;

		Vector3 point1 = {
			center.x + ( right.x * cosf(angle1) + up.x * sinf(angle1) ) * radius,
			center.y + ( right.y * cosf(angle1) + up.y * sinf(angle1) ) * radius,
			center.z + ( right.z * cosf(angle1) + up.z * sinf(angle1) ) * radius
		};

		Vector3 point2 = {
			center.x + ( right.x * cosf(angle2) + up.x * sinf(angle2) ) * radius,
			center.y + ( right.y * cosf(angle2) + up.y * sinf(angle2) ) * radius,
			center.z + ( right.z * cosf(angle2) + up.z * sinf(angle2) ) * radius
		};

		lineManager_->DrawLine(point1, point2, color);
	}
}

// カメラに正対する四角形を描画する補助関数
void Hud::DrawFacingSquare(const Vector3 &center, float size, const Vector4 &color, const Vector3 &cameraRight, const Vector3 &cameraUp) {
	// 四角形の各頂点を計算
	Vector3 topLeft = {
		center.x - cameraRight.x * size + cameraUp.x * size,
		center.y - cameraRight.y * size + cameraUp.y * size,
		center.z - cameraRight.z * size + cameraUp.z * size
	};

	Vector3 topRight = {
		center.x + cameraRight.x * size + cameraUp.x * size,
		center.y + cameraRight.y * size + cameraUp.y * size,
		center.z + cameraRight.z * size + cameraUp.z * size
	};

	Vector3 bottomLeft = {
		center.x - cameraRight.x * size - cameraUp.x * size,
		center.y - cameraRight.y * size - cameraUp.y * size,
		center.z - cameraRight.z * size - cameraUp.z * size
	};

	Vector3 bottomRight = {
		center.x + cameraRight.x * size - cameraUp.x * size,
		center.y + cameraRight.y * size - cameraUp.y * size,
		center.z + cameraRight.z * size - cameraUp.z * size
	};

	// 四角形の4辺を描画
	lineManager_->DrawLine(topLeft, topRight, color);
	lineManager_->DrawLine(topRight, bottomRight, color);
	lineManager_->DrawLine(bottomRight, bottomLeft, color);
	lineManager_->DrawLine(bottomLeft, topLeft, color);
}

// カメラに正対する進行度円弧を描画する補助関数
void Hud::DrawFacingProgressArc(const Vector3 &center, float radius, float startAngle, float endAngle,
	const Vector4 &color, int segments, const Vector3 &cameraRight, const Vector3 &cameraUp) {
// 円弧の分割点を計算して線分で描画
	for(int i = 0; i < segments; i++) {
		float angle1 = startAngle + ( endAngle - startAngle ) * static_cast<float>( i ) / segments;
		float angle2 = startAngle + ( endAngle - startAngle ) * static_cast<float>( i + 1 ) / segments;

		if(angle1 >= startAngle && angle1 <= endAngle && angle2 <= endAngle) {
			Vector3 point1 = {
				center.x + ( cameraRight.x * cosf(angle1) + cameraUp.x * sinf(angle1) ) * radius,
				center.y + ( cameraRight.y * cosf(angle1) + cameraUp.y * sinf(angle1) ) * radius,
				center.z + ( cameraRight.z * cosf(angle1) + cameraUp.z * sinf(angle1) ) * radius
			};

			Vector3 point2 = {
				center.x + ( cameraRight.x * cosf(angle2) + cameraUp.x * sinf(angle2) ) * radius,
				center.y + ( cameraRight.y * cosf(angle2) + cameraUp.y * sinf(angle2) ) * radius,
				center.z + ( cameraRight.z * cosf(angle2) + cameraUp.z * sinf(angle2) ) * radius
			};

			lineManager_->DrawLine(point1, point2, color);
		}
	}
}