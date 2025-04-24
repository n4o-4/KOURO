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

	// レーダー表示をアクティブにする
	showRadar_ = true;
}

///=============================================================================
///						敵コンテナとスポーンブロックの設定
void Hud::SetEnemiesAndSpawns(const std::vector<std::unique_ptr<BaseEnemy>> *enemies,
	const std::vector<std::unique_ptr<BaseEnemy>> *spawns) {
	enemies_ = enemies;
	spawns_ = spawns;
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

    // レーダーの回転をカメラの向きに合わせる
    if(rotateWithCamera_ && followCamera_) {
        // カメラの前方向ベクトルからY軸周りの回転角を計算
        Vector3 forward = followCamera_->GetForwardDirection();
        // Z軸を前として、X軸を右とした場合の角度を計算
        radarRotation_ = atan2f(forward.x, forward.z);
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

    // 3D球体ミニマップの描画（2Dレーダーの代わりに使用）
    if(use3DSphereMap_) {
        DrawSphereMap(viewProjection);
    }
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
void Hud::DrawCrosshair(ViewProjection viewProjection) {
    // プレイヤーの位置を取得
    Vector3 playerPos = followCamera_->GetViewProjection().worldPosition_;

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
    
    // 全天周囲モニター風の照準に更新
    
    // 内側の円を描画（サイト中心）
    DrawFacingCircle(center, crosshairCenterRadius_ * 0.5f, 
                     Vector4{1.0f, 1.0f, 1.0f, 0.9f}, crosshairCircleSegments_, cameraForward);
                     
    // 外側のリング
    DrawFacingCircle(center, crosshairSize_ * 1.0f, crosshairColor_, crosshairCircleSegments_ * 2, cameraForward);
    
    // 方向指示マーク（十字線の代わり）
    float markerSize = crosshairSize_ * 0.8f;
    float gapSize = crosshairGap_ * 0.7f;
    
    // 上マーカー（三角形）
    Vector3 topMarkerCenter = {
        center.x + upDir.x * gapSize,
        center.y + upDir.y * gapSize,
        center.z + upDir.z * gapSize
    };
    DrawFacingTriangle(topMarkerCenter, markerSize * 0.3f, crosshairColor_, cameraForward);
    
    // 下マーカー（四角形）
    Vector3 bottomMarkerCenter = {
        center.x - upDir.x * gapSize,
        center.y - upDir.y * gapSize,
        center.z - upDir.z * gapSize
    };
    DrawFacingSquare(bottomMarkerCenter, markerSize * 0.25f, crosshairColor_, rightDir, upDir);
    
    // 左右マーカー（線）
    float sideMarkerLength = markerSize * 0.4f;
    
    // 右マーカー
    Vector3 rightStart = {
        center.x + rightDir.x * gapSize,
        center.y + rightDir.y * gapSize,
        center.z + rightDir.z * gapSize
    };
    
    Vector3 rightEnd = {
        rightStart.x + rightDir.x * sideMarkerLength,
        rightStart.y + rightDir.y * sideMarkerLength,
        rightStart.z + rightDir.z * sideMarkerLength
    };
    
    lineManager_->DrawLine(rightStart, rightEnd, crosshairColor_);
    
    // 左マーカー
    Vector3 leftStart = {
        center.x - rightDir.x * gapSize,
        center.y - rightDir.y * gapSize,
        center.z - rightDir.z * gapSize
    };
    
    Vector3 leftEnd = {
        leftStart.x - rightDir.x * sideMarkerLength,
        leftStart.y - rightDir.y * sideMarkerLength,
        leftStart.z - rightDir.z * sideMarkerLength
    };
    
    lineManager_->DrawLine(leftStart, leftEnd, crosshairColor_);
    
    // ロックオンターゲットがある場合に照準に追加情報を表示
    if(lockOnSystem_) {
        const auto& lockedEnemies = lockOnSystem_->GetLockedEnemies();
        if(!lockedEnemies.empty()) {
            // 最初のロックオンターゲットの情報を照準に反映
            BaseEnemy* target = lockedEnemies[0];
            if(target) {
                LockOn::LockLevel lockLevel = lockOnSystem_->GetLockLevel(target);
                
                // ロックレベルに応じた表示
                if(lockLevel == LockOn::LockLevel::PreciseLock) {
                    // 精密ロック時は照準を拡張
                    float pulseSize = 1.0f + 0.2f * sinf(lockOnRotation_ * 5.0f);
                    DrawFacingCircle(center, crosshairSize_ * 1.5f * pulseSize, 
                                    hudLockColor_, crosshairCircleSegments_, cameraForward);
                                    
                    // 「ロック完了」を示す照準外枠
                    DrawFacingPolygon(center, crosshairSize_ * 2.0f, 8, hudLockColor_, cameraForward);
                }
                else if(lockLevel == LockOn::LockLevel::SimpleLock) {
                    // 簡易ロック時は照準に補助円を追加
                    DrawFacingCircle(center, crosshairSize_ * 1.2f, 
                                    hudAlertColor_, crosshairCircleSegments_, cameraForward);
                }
            }
        }
    }
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
			// 精密ロックオン（エースコンバットスタイル）
			markerColor = preciseLockonColor_;
			markerSize = lockOnSize_ * preciseLockonSizeRatio_;

			// 八角形のターゲットボックスを描画
			int segmentCount = 8;
			Vector3 prevPoint;
			for(int i = 0; i <= segmentCount; i++) {
				float segmentAngle = i * kTwoPi / segmentCount;
				Vector3 point = {
					enemyPos.x + cameraRight.x * cosf(segmentAngle) * markerSize + cameraUp.x * sinf(segmentAngle) * markerSize,
					enemyPos.y + cameraRight.y * cosf(segmentAngle) * markerSize + cameraUp.y * sinf(segmentAngle) * markerSize,
					enemyPos.z + cameraRight.z * cosf(segmentAngle) * markerSize + cameraUp.z * sinf(segmentAngle) * markerSize
				};

				if(i > 0) {
					lineManager_->DrawLine(prevPoint, point, markerColor);
				}
				prevPoint = point;
			}

			// 「LOCKED」テキストの代わりに中央に十字マークを表示
			float crossSize = markerSize * 0.3f;
			lineManager_->DrawLine(
				{ enemyPos.x - cameraRight.x * crossSize, enemyPos.y - cameraRight.y * crossSize, enemyPos.z - cameraRight.z * crossSize },
				{ enemyPos.x + cameraRight.x * crossSize, enemyPos.y + cameraRight.y * crossSize, enemyPos.z + cameraRight.z * crossSize },
				markerColor
			);
			lineManager_->DrawLine(
				{ enemyPos.x - cameraUp.x * crossSize, enemyPos.y - cameraUp.y * crossSize, enemyPos.z - cameraUp.z * crossSize },
				{ enemyPos.x + cameraUp.x * crossSize, enemyPos.y + cameraUp.y * crossSize, enemyPos.z + cameraUp.z * crossSize },
				markerColor
			);
			break;
		}
		case LockOn::LockLevel::SimpleLock:
		{
			// 簡易ロックオン（エースコンバットスタイル - 六角形）
			markerColor = simpleLockonColor_;
			markerSize = lockOnSize_ * simpleLockonSizeRatio_;

			// カメラに向いた六角形を描画
			int segmentCount = 6;
			Vector3 prevPoint;
			for(int i = 0; i <= segmentCount; i++) {
				float segmentAngle = i * kTwoPi / segmentCount;
				Vector3 point = {
					enemyPos.x + cameraRight.x * cosf(segmentAngle) * markerSize + cameraUp.x * sinf(segmentAngle) * markerSize,
					enemyPos.y + cameraRight.y * cosf(segmentAngle) * markerSize + cameraUp.y * sinf(segmentAngle) * markerSize,
					enemyPos.z + cameraRight.z * cosf(segmentAngle) * markerSize + cameraUp.z * sinf(segmentAngle) * markerSize
				};

				if(i > 0) {
					lineManager_->DrawLine(prevPoint, point, markerColor);
				}
				prevPoint = point;
			}
			break;
		}
		case LockOn::LockLevel::None:
		default:
		{
			// ロックオンなし/デフォルト（菱形マーカー）
			markerColor = noLockonColor_;
			markerSize = lockOnSize_ * noLockonSizeRatio_;

			// カメラの向きに合わせた菱形を描画
			Vector3 top = {
				enemyPos.x + cameraUp.x * markerSize,
				enemyPos.y + cameraUp.y * markerSize,
				enemyPos.z + cameraUp.z * markerSize
			};

			Vector3 right = {
				enemyPos.x + cameraRight.x * markerSize,
				enemyPos.y + cameraRight.y * markerSize,
				enemyPos.z + cameraRight.z * markerSize
			};

			Vector3 bottom = {
				enemyPos.x - cameraUp.x * markerSize,
				enemyPos.y - cameraUp.y * markerSize,
				enemyPos.z - cameraUp.z * markerSize
			};

			Vector3 left = {
				enemyPos.x - cameraRight.x * markerSize,
				enemyPos.y - cameraRight.y * markerSize,
				enemyPos.z - cameraRight.z * markerSize
			};

			lineManager_->DrawLine(top, right, markerColor);
			lineManager_->DrawLine(right, bottom, markerColor);
			lineManager_->DrawLine(bottom, left, markerColor);
			lineManager_->DrawLine(left, top, markerColor);
			break;
		}
		}

		// ロックオン進行状況インジケーター（エースコンバットスタイル - 四分割進行度）
		LockOn::LockedEnemyInfo *info = lockOnSystem_->GetLockedEnemyInfo(enemy);
		if(info && lockLevel == LockOn::LockLevel::SimpleLock) {
			// ロックオン進行度を計算（0.0～1.0）
			float lockProgress = info->lockTimer / lockOnSystem_->GetTimeToPreciseLock();
			lockProgress = std::min(lockProgress, 1.0f);

			// 進行度に応じて4つのコーナーを順に表示
			float cornerSize = markerSize * 1.2f;
			float cornerLength = markerSize * 0.8f;

			// 進行度に応じて表示するコーナーの数を決定
			int cornersToShow = static_cast<int>( lockProgress * 4.0f ) + 1;
			cornersToShow = std::min(cornersToShow, 4);

			// 左上コーナー
			if(cornersToShow >= 1) {
				Vector3 corner1Start = {
					enemyPos.x - cameraRight.x * cornerSize,
					enemyPos.y + cameraUp.y * cornerSize,
					enemyPos.z - cameraRight.z * cornerSize + cameraUp.z * cornerSize
				};
				Vector3 corner1HLine = {
					corner1Start.x + cameraRight.x * cornerLength,
					corner1Start.y,
					corner1Start.z + cameraRight.z * cornerLength
				};
				Vector3 corner1VLine = {
					corner1Start.x,
					corner1Start.y - cameraUp.y * cornerLength,
					corner1Start.z - cameraUp.z * cornerLength
				};
				lineManager_->DrawLine(corner1Start, corner1HLine, progressColor_);
				lineManager_->DrawLine(corner1Start, corner1VLine, progressColor_);
			}

			// 右上コーナー
			if(cornersToShow >= 2) {
				Vector3 corner2Start = {
					enemyPos.x + cameraRight.x * cornerSize,
					enemyPos.y + cameraUp.y * cornerSize,
					enemyPos.z + cameraRight.z * cornerSize + cameraUp.z * cornerSize
				};
				Vector3 corner2HLine = {
					corner2Start.x - cameraRight.x * cornerLength,
					corner2Start.y,
					corner2Start.z - cameraRight.z * cornerLength
				};
				Vector3 corner2VLine = {
					corner2Start.x,
					corner2Start.y - cameraUp.y * cornerLength,
					corner2Start.z - cameraUp.z * cornerLength
				};
				lineManager_->DrawLine(corner2Start, corner2HLine, progressColor_);
				lineManager_->DrawLine(corner2Start, corner2VLine, progressColor_);
			}

			// 右下コーナー
			if(cornersToShow >= 3) {
				Vector3 corner3Start = {
					enemyPos.x + cameraRight.x * cornerSize,
					enemyPos.y - cameraUp.y * cornerSize,
					enemyPos.z + cameraRight.z * cornerSize - cameraUp.z * cornerSize
				};
				Vector3 corner3HLine = {
					corner3Start.x - cameraRight.x * cornerLength,
					corner3Start.y,
					corner3Start.z - cameraRight.z * cornerLength
				};
				Vector3 corner3VLine = {
					corner3Start.x,
					corner3Start.y + cameraUp.y * cornerLength,
					corner3Start.z + cameraUp.z * cornerLength
				};
				lineManager_->DrawLine(corner3Start, corner3HLine, progressColor_);
				lineManager_->DrawLine(corner3Start, corner3VLine, progressColor_);
			}

			// 左下コーナー
			if(cornersToShow >= 4) {
				Vector3 corner4Start = {
					enemyPos.x - cameraRight.x * cornerSize,
					enemyPos.y - cameraUp.y * cornerSize,
					enemyPos.z - cameraRight.z * cornerSize - cameraUp.z * cornerSize
				};
				Vector3 corner4HLine = {
					corner4Start.x + cameraRight.x * cornerLength,
					corner4Start.y,
					corner4Start.z + cameraRight.z * cornerLength
				};
				Vector3 corner4VLine = {
					corner4Start.x,
					corner4Start.y + cameraUp.y * cornerLength,
					corner4Start.z + cameraUp.z * cornerLength
				};
				lineManager_->DrawLine(corner4Start, corner4HLine, progressColor_);
				lineManager_->DrawLine(corner4Start, corner4VLine, progressColor_);
			}
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

// カメラに正対する多角形を描画する補助関数を追加
void Hud::DrawFacingPolygon(const Vector3 &center, float size, int segments, const Vector4 &color, const Vector3 &cameraForward) {
	// カメラの前方向に垂直な平面上に多角形を描画
	Vector3 up = { 0.0f, 1.0f, 0.0f };
	// カメラの前方向とほぼ平行な場合は別の基準軸を使用
	if(fabsf(cameraForward.y) > 0.99f) {
		up = { 0.0f, 0.0f, 1.0f };
	}

	Vector3 right = Cross(up, cameraForward);
	Normalize(right);

	up = Cross(cameraForward, right);
	Normalize(up);

	// 多角形を描く
	std::vector<Vector3> points;
	for(int i = 0; i < segments; i++) {
		float angle = i * kTwoPi / segments;
		Vector3 point = {
			center.x + ( right.x * cosf(angle) + up.x * sinf(angle) ) * size,
			center.y + ( right.y * cosf(angle) + up.y * sinf(angle) ) * size,
			center.z + ( right.z * cosf(angle) + up.z * sinf(angle) ) * size
		};
		points.push_back(point);
	}

	// 各頂点を線で結ぶ
	for(int i = 0; i < segments; i++) {
		lineManager_->DrawLine(points[i], points[( i + 1 ) % segments], color);
	}
}

// カメラに正対する三角形を描画する補助関数
void Hud::DrawFacingTriangle(const Vector3 &center, float size, const Vector4 &color, const Vector3 &cameraForward) {
	// カメラの前方向に垂直な平面上に三角形を描画
	Vector3 up = { 0.0f, 1.0f, 0.0f };
	// カメラの前方向とほぼ平行な場合は別の基準軸を使用
	if(fabsf(cameraForward.y) > 0.99f) {
		up = { 0.0f, 0.0f, 1.0f };
	}

	Vector3 right = Cross(up, cameraForward);
	Normalize(right);

	up = Cross(cameraForward, right);
	Normalize(up);

	// 三角形の頂点を計算（上向き三角形）
	Vector3 top = {
		center.x + up.x * size,
		center.y + up.y * size,
		center.z + up.z * size
	};

	Vector3 bottomLeft = {
		center.x - right.x * size * 0.866f - up.x * size * 0.5f,
		center.y - right.y * size * 0.866f - up.y * size * 0.5f,
		center.z - right.z * size * 0.866f - up.z * size * 0.5f
	};

	Vector3 bottomRight = {
		center.x + right.x * size * 0.866f - up.x * size * 0.5f,
		center.y + right.y * size * 0.866f - up.y * size * 0.5f,
		center.z + right.z * size * 0.866f - up.z * size * 0.5f
	};

	// 三角形を描画
	lineManager_->DrawLine(top, bottomLeft, color);
	lineManager_->DrawLine(bottomLeft, bottomRight, color);
	lineManager_->DrawLine(bottomRight, top, color);
}

///=============================================================================
///						レーダーディスプレイの描画
void Hud::DrawRadarDisplay(ViewProjection viewProjection) {
	// プレイヤーの位置を取得
	Vector3 playerPos = followCamera_->GetViewProjection().worldPosition_;

	// カメラの位置と向きからレーダーの表示位置を計算
	Vector3 cameraForward = followCamera_->GetForwardDirection();
	Vector3 cameraRight = followCamera_->GetRightDirection();
	Vector3 cameraUp = followCamera_->GetUpDirection();

	// レーダーの位置を計算
	float halfSize = radarSize_ * 0.5f;
	Vector3 radarCenter = {
		playerPos.x + cameraRight.x * radarPositionX_ + cameraUp.x * radarPositionY_,
		playerPos.y + cameraRight.y * radarPositionX_ + cameraUp.y * radarPositionY_,
		playerPos.z + cameraRight.z * radarPositionX_ + cameraUp.z * radarPositionY_
	};

	// スイープ角度の更新
	radarSweepAngle_ += radarSweepSpeed_;
	if(radarSweepAngle_ > kTwoPi) {
		radarSweepAngle_ -= kTwoPi;
	}

	if(useCircularRadar_) {
		// エースコンバットスタイルの円形レーダー

		// 外側の円を描画
		DrawFacingCircle(radarCenter, halfSize, radarBorderColor_, radarCircleSegments_, cameraForward);

		// 内側の円を描画
		DrawFacingCircle(radarCenter, halfSize * radarInnerCircleRatio_, radarBorderColor_, radarCircleSegments_, cameraForward);

		// 十字線を描画
		lineManager_->DrawLine(
			{ radarCenter.x - halfSize, radarCenter.y, radarCenter.z },
			{ radarCenter.x + halfSize, radarCenter.y, radarCenter.z },
			radarGridColor_
		);
		lineManager_->DrawLine(
			{ radarCenter.x, radarCenter.y - halfSize, radarCenter.z },
			{ radarCenter.x, radarCenter.y + halfSize, radarCenter.z },
			radarGridColor_
		);

		// レーダースイープを描画
		if(showRadarSweep_) {
			Vector3 sweepEnd = {
				radarCenter.x + cosf(radarSweepAngle_) * halfSize,
				radarCenter.y + sinf(radarSweepAngle_) * halfSize,
				radarCenter.z
			};
			lineManager_->DrawLine(radarCenter, sweepEnd, radarSweepColor_);
		}
	} else {
		// 四角形レーダー（従来のもの）
		Vector3 topLeft = {
			radarCenter.x - cameraRight.x * halfSize + cameraUp.x * halfSize,
			radarCenter.y - cameraRight.y * halfSize + cameraUp.y * halfSize,
			radarCenter.z - cameraRight.z * halfSize + cameraUp.z * halfSize
		};

		Vector3 topRight = {
			radarCenter.x + cameraRight.x * halfSize + cameraUp.x * halfSize,
			radarCenter.y + cameraRight.y * halfSize + cameraUp.y * halfSize,
			radarCenter.z + cameraRight.z * halfSize + cameraUp.z * halfSize
		};

		Vector3 bottomLeft = {
			radarCenter.x - cameraRight.x * halfSize - cameraUp.x * halfSize,
			radarCenter.y - cameraRight.y * halfSize - cameraUp.y * halfSize,
			radarCenter.z - cameraRight.z * halfSize - cameraUp.z * halfSize
		};

		Vector3 bottomRight = {
			radarCenter.x + cameraRight.x * halfSize - cameraUp.x * halfSize,
			radarCenter.y + cameraRight.y * halfSize - cameraUp.y * halfSize,
			radarCenter.z + cameraRight.z * halfSize - cameraUp.z * halfSize
		};

		// レーダーの背景と枠を描画
		lineManager_->DrawLine(topLeft, topRight, radarBorderColor_);
		lineManager_->DrawLine(topRight, bottomRight, radarBorderColor_);
		lineManager_->DrawLine(bottomRight, bottomLeft, radarBorderColor_);
		lineManager_->DrawLine(bottomLeft, topLeft, radarBorderColor_);
	}

	// レーダー上にプレイヤーを中心点として表示
	Vector3 radarPlayerPos = radarCenter;

	// プレイヤーの位置を中心点として描画
	DrawFacingCircle(radarPlayerPos, 0.3f, hudBaseColor_, 8, cameraForward);

	// 敵をレーダー上に表示
	if(enemies_ && !enemies_->empty()) {
		for(const auto &enemy : *enemies_) {
			if(enemy) {
				// 敵の位置を取得
				Vector3 enemyPos = enemy->GetPosition();

				// プレイヤーから敵への相対位置を計算
				Vector3 relativePos = {
					enemyPos.x - playerPos.x,
					0.0f,  // Y座標は無視する（2Dレーダー）
					enemyPos.z - playerPos.z
				};

				// 距離を計算
				float distance = sqrt(relativePos.x * relativePos.x + relativePos.z * relativePos.z);

				// レーダーの範囲内にある場合のみ表示
				if(distance <= radarRange_) {
					// レーダー上での敵の位置を計算
					float scale = halfSize / radarRange_;

					// 敵の表示方法が円形レーダーか四角形レーダーかで変わる
					Vector3 radarEnemyPos;

					if(useCircularRadar_) {
						// 円形レーダーでの座標変換
						float angle = atan2f(relativePos.x, relativePos.z) - radarRotation_;
						float scaledDistance = distance * scale;

						// 円形レーダーの中に収まるように座標を計算
						if(scaledDistance > halfSize) {
							scaledDistance = halfSize;
						}

						float rotatedX = sinf(angle) * scaledDistance;
						float rotatedZ = cosf(angle) * scaledDistance;

						radarEnemyPos = {
							radarCenter.x + rotatedX,
							radarCenter.y + rotatedZ,
							radarCenter.z
						};
					} else {
						// 四角形レーダーでの座標変換（従来の方法）
						float angle = atan2f(relativePos.x, relativePos.z) - radarRotation_;
						float rotatedX = sinf(angle) * distance * scale;
						float rotatedZ = cosf(angle) * distance * scale;

						radarEnemyPos = {
							radarCenter.x + rotatedX,
							radarCenter.y + rotatedZ,
							radarCenter.z
						};
					}

					// 敵をレーダー上に表示するコードを修正
					// エースコンバットスタイルの敵表示（三角形）
					float markerSize = enemyDotSize_ * 1.2f;

					// 修正：カメラに正対する三角形を使用
					DrawFacingTriangle(radarEnemyPos, markerSize, enemyDotColor_, cameraForward);

					// 放射線の描画（オプション）
					if(distance < radarRange_ * 0.7f) {  // 近い敵だけ放射線を表示
                        //NOTE: ここでの放射線は、敵の位置からプレイヤーの位置に向かって描画されます。
						//lineManager_->DrawLine(radarPlayerPos, radarEnemyPos, radiationLineColor_);
					}
				}
			}
		}
	}

	// スポーンブロックをレーダー上に表示
	if(spawns_ && !spawns_->empty()) {
		for(const auto &spawn : *spawns_) {
			if(spawn) {
				// スポーンの位置を取得
				Vector3 spawnPos = spawn->GetPosition();

				// プレイヤーからスポーンへの相対位置を計算
				Vector3 relativePos = {
					spawnPos.x - playerPos.x,
					0.0f,
					spawnPos.z - playerPos.z
				};

				// 距離を計算
				float distance = sqrt(relativePos.x * relativePos.x + relativePos.z * relativePos.z);

				// レーダーの範囲内にある場合のみ表示
				if(distance <= radarRange_) {
					// 円形レーダーでの座標変換
					float scale = halfSize / radarRange_;
					float angle = atan2f(relativePos.x, relativePos.z) - radarRotation_;
					float scaledDistance = distance * scale;

					// 円形レーダーの中に収まるように座標を計算
					if(useCircularRadar_ && scaledDistance > halfSize) {
						scaledDistance = halfSize;
					}

					float rotatedX = sinf(angle) * ( useCircularRadar_ ? scaledDistance : distance * scale );
					float rotatedZ = cosf(angle) * ( useCircularRadar_ ? scaledDistance : distance * scale );

					Vector3 radarSpawnPos = {
						radarCenter.x + rotatedX,
						radarCenter.y + rotatedZ,
						radarCenter.z
					};

					// エースコンバットスタイルの六角形でスポーンを表示
					DrawFacingPolygon(radarSpawnPos, spawnBlockSize_, 6, spawnBlockColor_, cameraForward);

					// スポーンへの放射線（重要なポイントなのでより強調して表示）
					lineManager_->DrawLine(radarPlayerPos, radarSpawnPos, radiationLineColor_);
				}
			}
		}
	}
}

///=============================================================================
///						3D球体ミニマップの描画
void Hud::DrawSphereMap(ViewProjection viewProjection) {
    // プレイヤーの位置を取得
    Vector3 playerPos = followCamera_->GetViewProjection().worldPosition_;

    // カメラの位置と向きを取得
    Vector3 cameraForward = followCamera_->GetForwardDirection();
    Vector3 cameraRight = followCamera_->GetRightDirection();
    Vector3 cameraUp = followCamera_->GetUpDirection();

     // 右上に配置するように変更、Z座標も反映
	 Vector3 sphereCenter = {
        playerPos.x + cameraRight.x * sphereMapPositionX_ + cameraUp.x * sphereMapPositionY_ + cameraForward.x * sphereMapPositionZ_,
        playerPos.y + cameraRight.y * sphereMapPositionX_ + cameraUp.y * sphereMapPositionY_ + cameraForward.y * sphereMapPositionZ_,
        playerPos.z + cameraRight.z * sphereMapPositionX_ + cameraUp.z * sphereMapPositionY_ + cameraForward.z * sphereMapPositionZ_
    };
    
    // 背景の半透明円を描画（視認性向上）
    //DrawFacingCircle(sphereCenter, sphereMapRadius_ * 1.1f, 
    //                Vector4{0.0f, 0.1f, 0.0f, 0.3f}, 24, cameraForward);
    
    // Y軸回りの回転を更新（自動回転）
    sphereRotationY_ += sphereRotationSpeed_;
    if(sphereRotationY_ > kTwoPi) {
        sphereRotationY_ -= kTwoPi;
    }

    // TODO:グリッドを描画
    //DrawSphereGrid(sphereCenter, sphereMapRadius_, sphereMapColor_, cameraForward);

    // TODO:プレイヤーの位置を中心に特別なマーカーで表示
    //DrawFacingPolygon(sphereCenter, 1.0f, 8, hudBaseColor_, cameraForward);

    // TODO:視線方向を示す三角形を表示
    //Vector3 viewDirection = {
    //    sphereCenter.x + cameraForward.x * sphereMapRadius_,
    //    sphereCenter.y + cameraForward.y * sphereMapRadius_,
    //    sphereCenter.z + cameraForward.z * sphereMapRadius_
    //};
    //Draw3DTriangle(viewDirection, 2.0f, Vector4{1.0f, 1.0f, 1.0f, 0.9f}, cameraForward);

    // 敵を球体上に表示（より目立つように）
    if(enemies_ && !enemies_->empty()) {
        for(const auto& enemy : *enemies_) {
            if(enemy) {
                // 敵の位置を取得
                Vector3 enemyPos = enemy->GetPosition();
                
                // 敵の実世界座標を球体上の座標に変換
                Vector3 sphereEnemyPos = WorldToSpherePosition(enemyPos, sphereCenter, sphereMapRadius_, sphereMapRange_);
                
                // 検出範囲内の場合のみ表示
                if(sphereEnemyPos.x != 0.0f || sphereEnemyPos.y != 0.0f || sphereEnemyPos.z != 0.0f) {
                    // 敵の方向ベクトル
                    Vector3 enemyDirection = enemyPos - playerPos;
                    
                    // ロックオン状態の敵は特別な表示に
                    if(lockOnSystem_ && lockOnSystem_->IsLocked(enemy.get())) {
                        // パルス効果のためのサイズ変動
                        float pulseScale = 1.0f + 0.3f * sinf(lockOnRotation_ * 5.0f);
                        Draw3DTriangle(sphereEnemyPos, sphereObjectScale_ * 2.0f * pulseScale, hudLockColor_, enemyDirection);
                        
                        // ロックオン線を表示
                        lineManager_->DrawLine(sphereCenter, sphereEnemyPos, 
                            Vector4{hudLockColor_.x, hudLockColor_.y, hudLockColor_.z, 0.7f});
                    } else {
                        // 通常の敵表示（三角形）
                        Draw3DTriangle(sphereEnemyPos, sphereObjectScale_ * 1.5f, enemyDotColor_, enemyDirection);
                    }
                    
                    // 距離表示（数値の代わりに線の長さで表現）
                    float dist = Length(enemyPos - playerPos);
                    float lineLength = sphereMapRadius_ * 0.2f * (1.0f - std::min(dist / sphereMapRange_, 0.9f));
                    Vector3 distLine = Normalize(sphereEnemyPos - sphereCenter);
                    
                    Vector3 lineEnd = {
                        sphereEnemyPos.x + distLine.x * lineLength,
                        sphereEnemyPos.y + distLine.y * lineLength,
                        sphereEnemyPos.z + distLine.z * lineLength
                    };
                    
                    lineManager_->DrawLine(sphereEnemyPos, lineEnd, enemyDotColor_);
                }
            }
        }
    }

    // スポーンブロックを球体上に表示（より目立つように）
    if(spawns_ && !spawns_->empty()) {
        for(const auto& spawn : *spawns_) {
            if(spawn) {
                // スポーンの位置を取得
                Vector3 spawnPos = spawn->GetPosition();
                
                // スポーンブロックの実世界座標を球体上の座標に変換
                Vector3 sphereSpawnPos = WorldToSpherePosition(spawnPos, sphereCenter, sphereMapRadius_, sphereMapRange_);
                
                // 検出範囲内の場合のみ表示
                if(sphereSpawnPos.x != 0.0f || sphereSpawnPos.y != 0.0f || sphereSpawnPos.z != 0.0f) {
                    // 警告表示として点滅効果を追加
                    float blinkFactor = 0.5f + 0.5f * sinf(lockOnRotation_ * 4.0f);
                    Vector4 blinkColor = {
                        spawnBlockColor_.x,
                        spawnBlockColor_.y * blinkFactor,
                        spawnBlockColor_.z,
                        spawnBlockColor_.w
                    };
                    
                    // スポーンブロックを六角形で表示
                    Draw3DHexagon(sphereSpawnPos, sphereObjectScale_ * 1.8f, blinkColor, spawnPos - playerPos);
                    
                    // 警告リングを追加
                    DrawFacingCircle(sphereSpawnPos, sphereObjectScale_ * 2.5f, blinkColor, 12, cameraForward);
                }
            }
        }
    }
    
    // 視点方向と水平方向の輪を描画（高度計のような役割）
    float horizonRingRadius = sphereMapRadius_ * 0.7f;
    Vector3 horizonRingCenter = sphereCenter;
    Vector3 upVector = {0.0f, 1.0f, 0.0f};
    
    int segments = 36;
    for(int i = 0; i < segments; i++) {
        float angle1 = i * kTwoPi / segments;
        float angle2 = (i + 1) * kTwoPi / segments;
        
        Vector3 p1 = {
            horizonRingCenter.x + horizonRingRadius * cosf(angle1),
            horizonRingCenter.y,
            horizonRingCenter.z + horizonRingRadius * sinf(angle1)
        };
        
        Vector3 p2 = {
            horizonRingCenter.x + horizonRingRadius * cosf(angle2),
            horizonRingCenter.y,
            horizonRingCenter.z + horizonRingRadius * sinf(angle2)
        };
        
        lineManager_->DrawLine(p1, p2, Vector4{0.5f, 0.8f, 1.0f, 0.7f});
    }
}

// 球体のグリッドを描画する関数（全天周囲モニター風に強化）
void Hud::DrawSphereGrid(const Vector3& center, float radius, const Vector4& color, const Vector3& cameraForward) {
    // 経線（縦線）を描画
    for(int i = 0; i < sphereLongitudeCount_; i++) {
        float angle = i * kTwoPi / sphereLongitudeCount_;
        
        // Y軸回りの回転を適用
        angle += sphereRotationY_;
        
        // 緯度に沿って線を引く（より細かく分割）
        for(int j = 0; j < 24; j++) {
            float phi1 = j * kPi / 24.0f;
            float phi2 = (j + 1) * kPi / 24.0f;
            
            // 球面上の点を計算
            Vector3 p1 = {
                center.x + radius * sinf(phi1) * cosf(angle),
                center.y + radius * cosf(phi1),
                center.z + radius * sinf(phi1) * sinf(angle)
            };
            
            Vector3 p2 = {
                center.x + radius * sinf(phi2) * cosf(angle),
                center.y + radius * cosf(phi2),
                center.z + radius * sinf(phi2) * sinf(angle)
            };
            
            // 経線の色を方位によって変える（北・南・東・西方向を強調）
            Vector4 lineColor = color;
            if(i % (sphereLongitudeCount_ / 4) == 0) {
                lineColor = Vector4{0.9f, 0.9f, 0.3f, 0.8f}; // 主方位は黄色で強調
            }
            
            lineManager_->DrawLine(p1, p2, lineColor);
        }
    }
    
    // 緯線（横線）を描画（より細かく）
    for(int i = 1; i <= sphereLatitudeCount_ * 2; i++) {
        float phi = i * kPi / (sphereLatitudeCount_ * 2 + 1);
        
        // 緯度の色を設定（赤道付近を強調）
        Vector4 lineColor = color;
        if(i == sphereLatitudeCount_) {
            lineColor = Vector4{0.9f, 0.3f, 0.3f, 0.8f}; // 赤道は赤で強調
        }
        
        for(int j = 0; j < 48; j++) {
            float angle1 = j * kTwoPi / 48.0f;
            float angle2 = (j + 1) * kTwoPi / 48.0f;
            
            // Y軸回りの回転を適用
            angle1 += sphereRotationY_;
            angle2 += sphereRotationY_;
            
            Vector3 p1 = {
                center.x + radius * sinf(phi) * cosf(angle1),
                center.y + radius * cosf(phi),
                center.z + radius * sinf(phi) * sinf(angle1)
            };
            
            Vector3 p2 = {
                center.x + radius * sinf(phi) * cosf(angle2),
                center.y + radius * cosf(phi),
                center.z + radius * sinf(phi) * sinf(angle2)
            };
            
            lineManager_->DrawLine(p1, p2, lineColor);
        }
    }
    
    // 球体の外側に方位マーカーを追加
    //const char* directions[4] = {"N", "E", "S", "W"};
    //for(int i = 0; i < 4; i++) {
    //    float angle = i * kPi / 2.0f + sphereRotationY_;
    //    Vector3 markerPos = {
    //        center.x + radius * 1.1f * sinf(kPi/2) * cosf(angle),
    //        center.y,
    //        center.z + radius * 1.1f * sinf(kPi/2) * sinf(angle)
    //    };
    //    
    //    // 方位マーカーとして円を描画（実際のテキスト描画はできないため）
    //    if(i == 0) { // 北
    //        DrawFacingCircle(markerPos, 1.5f, Vector4{0.9f, 0.9f, 0.3f, 0.8f}, 8, cameraForward);
    //    } else if(i == 2) { // 南
    //        DrawFacingTriangle(markerPos, 1.5f, Vector4{0.9f, 0.3f, 0.3f, 0.8f}, cameraForward);
    //    } else { // 東西
    //        DrawFacingSquare(markerPos, 1.2f, Vector4{0.3f, 0.9f, 0.9f, 0.8f}, 
    //                          followCamera_->GetRightDirection(), followCamera_->GetUpDirection());
    //    }
    //}
}

// 世界座標を球体マップ上の座標に変換する関数
Vector3 Hud::WorldToSpherePosition(const Vector3& worldPos, const Vector3& sphereCenter, float radius, float maxRange) {
    // プレイヤーの位置を取得
    Vector3 playerPos = followCamera_->GetViewProjection().worldPosition_;
    
    // プレイヤーから対象への相対ベクトル
    Vector3 relativePos = {
        worldPos.x - playerPos.x,
        worldPos.y - playerPos.y,
        worldPos.z - playerPos.z
    };
    
    // 距離を計算
    float distance = sqrtf(relativePos.x * relativePos.x + relativePos.y * relativePos.y + relativePos.z * relativePos.z);
    
    // 検出範囲外なら原点を返す（表示しない）
    if(distance > maxRange) {
        return {0.0f, 0.0f, 0.0f};
    }
    
    // 方向ベクトルを正規化
    Vector3 direction = {
        relativePos.x / distance,
        relativePos.y / distance,
        relativePos.z / distance
    };
    
    // 球面上の位置を計算（方向ベクトルに半径を掛ける）
    Vector3 spherePos = {
        sphereCenter.x + direction.x * radius,
        sphereCenter.y + direction.y * radius,
        sphereCenter.z + direction.z * radius
    };
    
    return spherePos;
}

// 3D空間に三角形を描画する関数（敵表示用）
void Hud::Draw3DTriangle(const Vector3& center, float size, const Vector4& color, const Vector3& direction) {
    // 方向ベクトルを正規化
    Vector3 dir = direction;
    float length = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    if(length > 0.0f) {
        dir.x /= length;
        dir.y /= length;
        dir.z /= length;
    } else {
        dir = {0.0f, 1.0f, 0.0f}; // デフォルト方向
    }
    
    // 三角形の頂点を計算するための基底ベクトルを作成
    Vector3 up = {0.0f, 1.0f, 0.0f};
    
    // dirとupがほぼ平行な場合は別の基準軸を使用
    if(fabsf(dir.y) > 0.99f) {
        up = {0.0f, 0.0f, 1.0f};
    }
    
    Vector3 right = Cross(up, dir);
    Normalize(right);
    
    up = Cross(dir, right);
    Normalize(up);
    
    // 三角形の頂点を計算（上向き三角形）
    Vector3 top = {
        center.x + dir.x * size,
        center.y + dir.y * size,
        center.z + dir.z * size
    };
    
    Vector3 bottomLeft = {
        center.x - right.x * size * 0.5f - dir.x * size * 0.5f,
        center.y - right.y * size * 0.5f - dir.y * size * 0.5f,
        center.z - right.z * size * 0.5f - dir.z * size * 0.5f
    };
    
    Vector3 bottomRight = {
        center.x + right.x * size * 0.5f - dir.x * size * 0.5f,
        center.y + right.y * size * 0.5f - dir.y * size * 0.5f,
        center.z + right.z * size * 0.5f - dir.z * size * 0.5f
    };
    
    // 三角形を描画
    lineManager_->DrawLine(top, bottomLeft, color);
    lineManager_->DrawLine(bottomLeft, bottomRight, color);
    lineManager_->DrawLine(bottomRight, top, color);
}

// 3D空間に六角形を描画する関数（スポーンブロック表示用）
void Hud::Draw3DHexagon(const Vector3& center, float size, const Vector4& color, const Vector3& direction) {
    // 方向ベクトルを正規化
    Vector3 dir = direction;
    float length = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
    if(length > 0.0f) {
        dir.x /= length;
        dir.y /= length;
        dir.z /= length;
    } else {
        dir = {0.0f, 1.0f, 0.0f}; // デフォルト方向
    }
    
    // 六角形の頂点を計算するための基底ベクトルを作成
    Vector3 up = {0.0f, 1.0f, 0.0f};
    
    // dirとupがほぼ平行な場合は別の基準軸を使用
    if(fabsf(dir.y) > 0.99f) {
        up = {0.0f, 0.0f, 1.0f};
    }
    
    Vector3 right = Cross(up, dir);
    Normalize(right);
    
    up = Cross(dir, right);
    Normalize(up);
    
    // 六角形の各頂点を計算
    const int segments = 6;
    std::vector<Vector3> vertices;
    
    for(int i = 0; i < segments; i++) {
        float angle = i * kTwoPi / segments;
        Vector3 vertex = {
            center.x + right.x * cosf(angle) * size + up.x * sinf(angle) * size,
            center.y + right.y * cosf(angle) * size + up.y * sinf(angle) * size,
            center.z + right.z * cosf(angle) * size + up.z * sinf(angle) * size
        };
        vertices.push_back(vertex);
    }
    
    // 六角形を描画
    for(int i = 0; i < segments; i++) {
        lineManager_->DrawLine(vertices[i], vertices[(i + 1) % segments], color);
    }
}

// ...existing code...

void Hud::DrawImGui() {
    // ImGuiウィンドウを開始
    ImGui::Begin("HUD Settings");
    
    // 戦闘モード設定
    ImGui::Checkbox("Combat Mode", &isCombatMode_);
    ImGui::Checkbox("Use 3D Sphere Map", &use3DSphereMap_);
    
    // 球体ミニマップ設定
    if (ImGui::CollapsingHeader("Sphere Map Settings")) {
        ImGui::SliderFloat("Radius", &sphereMapRadius_, 1.0f, 30.0f);
        ImGui::SliderFloat("Position X", &sphereMapPositionX_, -30.0f, 30.0f);
        ImGui::SliderFloat("Position Y", &sphereMapPositionY_, -30.0f, 30.0f);
		ImGui::SliderFloat("Position Z", &sphereMapPositionZ_, -30.0f, 30.0f);
        ImGui::SliderFloat("Range", &sphereMapRange_, 50.0f, 1000.0f);
        ImGui::SliderInt("Longitude Count", &sphereLongitudeCount_, 4, 20);
        ImGui::SliderInt("Latitude Count", &sphereLatitudeCount_, 2, 10);
        //ImGui::SliderFloat("Rotation Speed", &sphereRotationSpeed_, 0.0f, 0.05f);
        ImGui::SliderFloat("Object Scale", &sphereObjectScale_, 0.5f, 3.0f);
        //ImGui::ColorEdit4("Grid Color", &sphereGridColor_.x);
        ImGui::ColorEdit4("Map Color", &sphereMapColor_.x);
        ImGui::ColorEdit4("Lock Color", &sphereMapLockColor_.x);
    }
    
    // 照準設定
    if (ImGui::CollapsingHeader("Crosshair Settings")) {
        ImGui::SliderFloat("Size", &crosshairSize_, 1.0f, 10.0f);
        ImGui::SliderFloat("Distance", &crosshairDistance_, 10.0f, 200.0f);
        ImGui::SliderFloat("Gap", &crosshairGap_, 1.0f, 20.0f);
        ImGui::SliderFloat("Center Radius", &crosshairCenterRadius_, 0.1f, 5.0f);
        ImGui::SliderInt("Circle Segments", &crosshairCircleSegments_, 4, 32);
        ImGui::ColorEdit4("Color", &crosshairColor_.x);
    }
    
    // ロックオン設定
    if (ImGui::CollapsingHeader("Lock-on Settings")) {
        ImGui::SliderFloat("Size", &lockOnSize_, 1.0f, 15.0f);
        ImGui::SliderFloat("Rotation Speed", &lockOnRotationSpeed_, 0.001f, 0.1f);
        ImGui::ColorEdit4("Base Color", &lockOnColor_.x);
        ImGui::ColorEdit4("Precise Lock Color", &preciseLockonColor_.x);
        ImGui::ColorEdit4("Simple Lock Color", &simpleLockonColor_.x);
        ImGui::ColorEdit4("No Lock Color", &noLockonColor_.x);
        ImGui::ColorEdit4("Progress Color", &progressColor_.x);
    }
    
    // 全体カラー設定
    if (ImGui::CollapsingHeader("HUD Colors")) {
        ImGui::ColorEdit4("Base Color", &hudBaseColor_.x);
        ImGui::ColorEdit4("Accent Color", &hudAccentColor_.x);
        ImGui::ColorEdit4("Enemy Color", &hudEnemyColor_.x);
        ImGui::ColorEdit4("Alert Color", &hudAlertColor_.x);
        ImGui::ColorEdit4("Lock Color", &hudLockColor_.x);
        ImGui::ColorEdit4("Enemy Dot Color", &enemyDotColor_.x);
        ImGui::ColorEdit4("Spawn Block Color", &spawnBlockColor_.x);
    }
    
    // 敵とスポーン設定
    if (ImGui::CollapsingHeader("Enemy & Spawn Settings")) {
        ImGui::SliderFloat("Enemy Dot Size", &enemyDotSize_, 0.1f, 3.0f);
        ImGui::SliderFloat("Spawn Block Size", &spawnBlockSize_, 0.1f, 3.0f);
        ImGui::ColorEdit4("Radiation Line", &radiationLineColor_.x);
    }
    
    ImGui::End();
};
