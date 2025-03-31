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

	// レーダーディスプレイの描画
	if(showRadar_) {
		DrawRadarDisplay(viewProjection);
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
	Vector3 playerPos = player_->GetPosition();

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