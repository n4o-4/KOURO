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

    // 半透明グリッドラインの描画（最背面）
    DrawGridLines();

    // 基本情報パネル
    DrawSpeedIndicator();
    DrawAltitudeIndicator();
    DrawArtificialHorizon();
    DrawGForceIndicator();

    // 戦闘情報パネル
    DrawCrosshair(viewProjection);
	DrawLockOn(viewProjection);
    DrawMissileWarning();
    DrawIFFSystem(viewProjection);
    DrawWeaponIndicator();

    // 空間認識補助
    DrawRadarDisplay();
    DrawGroundWarning();
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
    // プレイヤーの前方位置を計算
    Vector3 forward = followCamera_->GetForwardDirection();
	Vector3 playerPos = player_->GetPosition();

	// カメラの前方向ベクトルを取得
	Vector3 cameraForward = followCamera_->GetForwardDirection();

	// 照準の位置を計算（プレイヤーの少し前方）
	Vector3 crosshairPos = {
        playerPos.x + forward.x * crosshairDistance_,
        playerPos.y + forward.y * crosshairDistance_,
        playerPos.z + forward.z * crosshairDistance_
	};

    // カメラの右方向と上方向を取得
    Vector3 right = followCamera_->GetRightDirection();
    Vector3 up = followCamera_->GetUpDirection();

    // 中央の円を描画
    DrawFacingCircle(crosshairPos, crosshairCenterRadius_, crosshairColor_, crosshairCircleSegments_, forward);

    // 十字線を描画（中央にギャップあり）
    // 右
	Vector3 rightStart = {
        crosshairPos.x + right.x * crosshairGap_,
        crosshairPos.y + right.y * crosshairGap_,
        crosshairPos.z + right.z * crosshairGap_
	};

	Vector3 rightEnd = {
        crosshairPos.x + right.x * (crosshairGap_ + crosshairSize_),
        crosshairPos.y + right.y * (crosshairGap_ + crosshairSize_),
        crosshairPos.z + right.z * (crosshairGap_ + crosshairSize_)
	};

	lineManager_->DrawLine(rightStart, rightEnd, crosshairColor_);

    // 左
	Vector3 leftStart = {
        crosshairPos.x - right.x * crosshairGap_,
        crosshairPos.y - right.y * crosshairGap_,
        crosshairPos.z - right.z * crosshairGap_
	};

	Vector3 leftEnd = {
        crosshairPos.x - right.x * (crosshairGap_ + crosshairSize_),
        crosshairPos.y - right.y * (crosshairGap_ + crosshairSize_),
        crosshairPos.z - right.z * (crosshairGap_ + crosshairSize_)
	};

	lineManager_->DrawLine(leftStart, leftEnd, crosshairColor_);

    // 上
	Vector3 upStart = {
        crosshairPos.x + up.x * crosshairGap_,
        crosshairPos.y + up.y * crosshairGap_,
        crosshairPos.z + up.z * crosshairGap_
	};

	Vector3 upEnd = {
        crosshairPos.x + up.x * (crosshairGap_ + crosshairSize_),
        crosshairPos.y + up.y * (crosshairGap_ + crosshairSize_),
        crosshairPos.z + up.z * (crosshairGap_ + crosshairSize_)
	};

	lineManager_->DrawLine(upStart, upEnd, crosshairColor_);

    // 下
	Vector3 downStart = {
        crosshairPos.x - up.x * crosshairGap_,
        crosshairPos.y - up.y * crosshairGap_,
        crosshairPos.z - up.z * crosshairGap_
	};

	Vector3 downEnd = {
        crosshairPos.x - up.x * (crosshairGap_ + crosshairSize_),
        crosshairPos.y - up.y * (crosshairGap_ + crosshairSize_),
        crosshairPos.z - up.z * (crosshairGap_ + crosshairSize_)
	};

	lineManager_->DrawLine(downStart, downEnd, crosshairColor_);

	// 中央の小さい円
	lineManager_->DrawCircle(center, crosshairCenterRadius_, crosshairColor_, crosshairCircleSegments_);
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

// ヘルパー関数：2D位置に縦バーを描画
void Hud::DrawBar(const Vector2& position, float width, float height, float fillAmount, const Vector4& color) {
    // バーの高さを塗りつぶし量に基づいて計算
    float fillHeight = height * fillAmount;
    
    // バーの外枠（四角形）を描画
    Vector2 topLeft = {position.x - width/2, position.y + height/2};
    Vector2 topRight = {position.x + width/2, position.y + height/2};
    Vector2 bottomLeft = {position.x - width/2, position.y - height/2};
    Vector2 bottomRight = {position.x + width/2, position.y - height/2};
    
    lineManager_->DrawLine({topLeft.x, topLeft.y, 0}, {topRight.x, topRight.y, 0}, color);
    lineManager_->DrawLine({topRight.x, topRight.y, 0}, {bottomRight.x, bottomRight.y, 0}, color);
    lineManager_->DrawLine({bottomRight.x, bottomRight.y, 0}, {bottomLeft.x, bottomLeft.y, 0}, color);
    lineManager_->DrawLine({bottomLeft.x, bottomLeft.y, 0}, {topLeft.x, topLeft.y, 0}, color);
    
    // バーの塗りつぶし部分（下から上へ塗りつぶす）
    Vector2 fillTopLeft = {position.x - width/2, position.y - height/2 + fillHeight};
    Vector2 fillTopRight = {position.x + width/2, position.y - height/2 + fillHeight};
    
    // 内部を塗りつぶすための横線を描画
    for (float y = position.y - height/2; y < position.y - height/2 + fillHeight; y += 1.0f) {
        lineManager_->DrawLine(
            {position.x - width/2, y, 0},
            {position.x + width/2, y, 0},
            color
        );
    }
}

// ヘルパー関数：点線の描画
void Hud::DrawDottedLine(const Vector2& start, const Vector2& end, const Vector4& color, float dashLength, float gapLength) {
    Vector2 direction = {end.x - start.x, end.y - start.y};
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    
    // 単位方向ベクトルを計算
    direction = {direction.x / length, direction.y / length};
    
    float currentLength = 0.0f;
    bool drawLine = true; // 最初は線を描画
    
    while (currentLength < length) {
        float remainingLength = length - currentLength;
        float segmentLength = drawLine ? dashLength : gapLength;
        
        if (segmentLength > remainingLength) {
            segmentLength = remainingLength;
        }
        
        if (drawLine) {
            Vector2 segStart = {
                start.x + direction.x * currentLength,
                start.y + direction.y * currentLength
            };
            
            Vector2 segEnd = {
                start.x + direction.x * (currentLength + segmentLength),
                start.y + direction.y * (currentLength + segmentLength)
            };
            
            lineManager_->DrawLine({segStart.x, segStart.y, 0}, {segEnd.x, segEnd.y, 0}, color);
        }
        
        currentLength += segmentLength;
        drawLine = !drawLine;
	}
}

// ヘルパー関数：円弧の描画
void Hud::DrawArc(const Vector2& center, float radius, float startAngle, float endAngle, const Vector4& color, int segments) {
    float angleStep = (endAngle - startAngle) / static_cast<float>(segments);
    
    for (int i = 0; i < segments; i++) {
        float angle1 = startAngle + angleStep * i;
        float angle2 = startAngle + angleStep * (i + 1);
        
        Vector2 p1 = {
            center.x + radius * cosf(angle1),
            center.y + radius * sinf(angle1)
        };
        
        Vector2 p2 = {
            center.x + radius * cosf(angle2),
            center.y + radius * sinf(angle2)
        };
        
        lineManager_->DrawLine({p1.x, p1.y, 0}, {p2.x, p2.y, 0}, color);
    }
}

///=============================================================================
/// 速度メーターの描画
void Hud::DrawSpeedIndicator() {
    // プレイヤーの速度を取得（0～1の範囲に正規化）
    float speed = player_->GetVelocity().x / maxSpeed_;
    speed = std::clamp(speed, 0.0f, 1.0f);
    
    // 背景バーを描画
    DrawBar(speedIndicatorPosition_, speedIndicatorWidth_, speedIndicatorHeight_, 1.0f, speedBackgroundColor_);
    
    // 速度バーを描画
    DrawBar(speedIndicatorPosition_, speedIndicatorWidth_, speedIndicatorHeight_, speed, speedIndicatorColor_);
    
    // 単位などのテキスト（実際のテキスト描画はクライアントの実装に依存）
    // ここでは速度の目盛りを10分割して描画
    for (int i = 0; i <= 10; i++) {
        float y = speedIndicatorPosition_.y - speedIndicatorHeight_/2 + i * (speedIndicatorHeight_/10);
        float tickSize = (i % 5 == 0) ? 6.0f : 3.0f; // 5の倍数はより長い目盛り
        
        lineManager_->DrawLine(
            {speedIndicatorPosition_.x - tickSize, y, 0},
            {speedIndicatorPosition_.x - speedIndicatorWidth_/2, y, 0},
            speedIndicatorColor_
        );
    }
}

///=============================================================================
/// 高度メーターの描画
void Hud::DrawAltitudeIndicator() {
    // プレイヤーの高度を取得（0～1の範囲に正規化）
    float altitude = player_->GetPosition().y / maxAltitude_;
    altitude = std::clamp(altitude, 0.0f, 1.0f);
    
    // 背景バーを描画
    DrawBar(altitudeIndicatorPosition_, altitudeIndicatorWidth_, altitudeIndicatorHeight_, 1.0f, altitudeBackgroundColor_);
    
    // 高度バーを描画
    DrawBar(altitudeIndicatorPosition_, altitudeIndicatorWidth_, altitudeIndicatorHeight_, altitude, altitudeIndicatorColor_);
    
    // 単位などのテキスト（実際のテキスト描画はクライアントの実装に依存）
    // ここでは高度の目盛りを10分割して描画
    for (int i = 0; i <= 10; i++) {
        float y = altitudeIndicatorPosition_.y - altitudeIndicatorHeight_/2 + i * (altitudeIndicatorHeight_/10);
        float tickSize = (i % 5 == 0) ? 6.0f : 3.0f; // 5の倍数はより長い目盛り
        
        lineManager_->DrawLine(
            {altitudeIndicatorPosition_.x + tickSize, y, 0},
            {altitudeIndicatorPosition_.x + altitudeIndicatorWidth_/2, y, 0},
            altitudeIndicatorColor_
        );
    }
}

///=============================================================================
/// 人工水平線の描画
void Hud::DrawArtificialHorizon() {
    // プレイヤーの回転（ロール角）を取得
    float roll = player_->GetRotation().z;
    
    // 人工水平線の中心位置
    Vector2 center = {0.0f, 0.0f}; // 画面中央
    
    // 回転した水平線の端点を計算
    float cosRoll = cosf(roll);
    float sinRoll = sinf(roll);
    
    Vector2 leftPoint = {
        center.x - horizonLength_ * cosRoll,
        center.y - horizonLength_ * sinRoll
    };
    
    Vector2 rightPoint = {
        center.x + horizonLength_ * cosRoll,
        center.y + horizonLength_ * sinRoll
    };
    
    // メイン水平線を描画
    lineManager_->DrawLine({leftPoint.x, leftPoint.y, 0}, {rightPoint.x, rightPoint.y, 0}, horizonColor_);
    
    // 参照線を描画（上下に数本の線を追加）
    for (int i = 1; i <= horizonSteps_; i++) {
        // 上側の参照線
        Vector2 upOffset = {
            -horizonStepSize_ * i * sinRoll,
            horizonStepSize_ * i * cosRoll
        };
        
        Vector2 upLeftPoint = {leftPoint.x + upOffset.x, leftPoint.y + upOffset.y};
        Vector2 upRightPoint = {rightPoint.x + upOffset.x, rightPoint.y + upOffset.y};
        
        DrawDottedLine(upLeftPoint, upRightPoint, horizonReferenceColor_, 5.0f, 3.0f);
        
        // 下側の参照線
        Vector2 downOffset = {
            horizonStepSize_ * i * sinRoll,
            -horizonStepSize_ * i * cosRoll
        };
        
        Vector2 downLeftPoint = {leftPoint.x + downOffset.x, leftPoint.y + downOffset.y};
        Vector2 downRightPoint = {rightPoint.x + downOffset.x, rightPoint.y + downOffset.y};
        
        DrawDottedLine(downLeftPoint, downRightPoint, horizonReferenceColor_, 5.0f, 3.0f);
    }
}

///=============================================================================
/// Gメーターの描画
void Hud::DrawGForceIndicator() {
    // プレイヤーの速度変化からG値を計算（実際の計算はもっと複雑）
    Vector3 velocity = player_->GetVelocity();
    Vector3 prevVelocity = player_->GetVelocity();
    Vector3 acceleration = {
        velocity.x - prevVelocity.x,
        velocity.y - prevVelocity.y,
        velocity.z - prevVelocity.z
    };
    
    // 加速度の大きさからG値を計算
    float accelMagnitude = sqrtf(
        acceleration.x * acceleration.x +
        acceleration.y * acceleration.y +
        acceleration.z * acceleration.z
    );
    
    // G値を更新（平滑化）
    float targetGForce = accelMagnitude / 9.8f; // 重力加速度で割る
    currentGForce_ = currentGForce_ * (1.0f - gForceSmoothingFactor_) + targetGForce * gForceSmoothingFactor_;
    
    // G値を0～1の範囲に正規化
    float normalizedGForce = currentGForce_ / maxGForce_;
    normalizedGForce = std::clamp(normalizedGForce, 0.0f, 1.0f);
    
    // 半円形のメーター
    // 背景の半円
    DrawArc(
        gForcePosition_,
        gForceRadius_,
        0.0f,
        kPi,
        gForceBackgroundColor_,
        32
    );
    
    // G値に応じた弧を描画
    DrawArc(
        gForcePosition_,
        gForceRadius_,
        0.0f,
        kPi * normalizedGForce,
        gForceColor_,
        32
    );
    
    // G値が大きい場合、より太い弧を外側に追加
    if (normalizedGForce > 0.5f) {
        float outerRadius = gForceRadius_ * 1.1f;
        float intensityFactor = (normalizedGForce - 0.5f) * 2.0f; // 0.5を超えた分を0-1に正規化
        Vector4 intensityColor = gForceColor_;
        intensityColor.w *= intensityFactor; // アルファ値を強度に合わせて調整
        
        DrawArc(
            gForcePosition_,
            outerRadius,
            0.0f,
            kPi * normalizedGForce,
            intensityColor,
            32
        );
    }
    
    // G値の目盛りを描画
    for (int i = 0; i <= 10; i++) {
        float angle = kPi * i / 10.0f;
        float innerX = gForcePosition_.x + gForceRadius_ * 0.9f * cosf(angle);
        float innerY = gForcePosition_.y + gForceRadius_ * 0.9f * sinf(angle);
        float outerX = gForcePosition_.x + gForceRadius_ * cosf(angle);
        float outerY = gForcePosition_.y + gForceRadius_ * sinf(angle);
        
        lineManager_->DrawLine(
            {innerX, innerY, 0},
            {outerX, outerY, 0},
            gForceColor_
        );
    }
}

///=============================================================================
/// ミサイル警報の描画
void Hud::DrawMissileWarning() {
    // ミサイル警報がアクティブでない場合は描画しない
    if (!isMissileWarning_) {
        return;
    }
    
    // 警告表示を点滅させる
    missileWarningTimer_ += 0.016f; // 仮の時間増分
    if (missileWarningTimer_ > missileWarningBlinkRate_) {
        missileWarningTimer_ -= missileWarningBlinkRate_;
    }
    
    // 点滅中の非表示期間の場合は描画しない
    if (missileWarningTimer_ > missileWarningBlinkRate_ * 0.5f) {
        return;
    }
    
    // 画面の四隅に三角形を描画
    float halfWidth = 400.0f; // 画面幅の半分（調整要）
    float halfHeight = 300.0f; // 画面高さの半分（調整要）
    
    // 左上
    Vector2 topLeft = {-halfWidth + 50.0f, halfHeight - 50.0f};
    DrawWarningTriangle(topLeft, 30.0f, 3.14159f * 0.25f, missileWarningColor_);
    
    // 右上
    Vector2 topRight = {halfWidth - 50.0f, halfHeight - 50.0f};
    DrawWarningTriangle(topRight, 30.0f, 3.14159f * 0.75f, missileWarningColor_);
    
    // 右下
    Vector2 bottomRight = {halfWidth - 50.0f, -halfHeight + 50.0f};
    DrawWarningTriangle(bottomRight, 30.0f, 3.14159f * 1.25f, missileWarningColor_);
    
    // 左下
    Vector2 bottomLeft = {-halfWidth + 50.0f, -halfHeight + 50.0f};
    DrawWarningTriangle(bottomLeft, 30.0f, 3.14159f * 1.75f, missileWarningColor_);
    
    // 「MISSILE」警告テキスト（点線で表現）
    Vector2 textPos = {0.0f, halfHeight - 30.0f}; // 画面上部中央
    DrawDottedLine(
        {textPos.x - 80.0f, textPos.y},
        {textPos.x + 80.0f, textPos.y},
        missileWarningColor_,
        10.0f,
        5.0f
    );
}

// 警告三角形の描画ヘルパー関数
void Hud::DrawWarningTriangle(const Vector2& center, float size, float rotation, const Vector4& color) {
    float cosRot = cosf(rotation);
    float sinRot = sinf(rotation);
    
    // 三角形の頂点を計算
    Vector2 p1 = {
        center.x + size * cosRot,
        center.y + size * sinRot
    };
    
    Vector2 p2 = {
        center.x + size * cosf(rotation + 2.0f * kPi / 3.0f),
        center.y + size * sinf(rotation + 2.0f * kPi / 3.0f)
    };
    
    Vector2 p3 = {
        center.x + size * cosf(rotation + 4.0f * kPi / 3.0f),
        center.y + size * sinf(rotation + 4.0f * kPi / 3.0f)
    };
    
    // 三角形の辺を点線で描画
    DrawDottedLine({p1.x, p1.y}, {p2.x, p2.y}, color, 5.0f, 3.0f);
    DrawDottedLine({p2.x, p2.y}, {p3.x, p3.y}, color, 5.0f, 3.0f);
    DrawDottedLine({p3.x, p3.y}, {p1.x, p1.y}, color, 5.0f, 3.0f);
}

///=============================================================================
/// 敵機の表示（IFFシステム）
void Hud::DrawIFFSystem(ViewProjection viewProjection) {
    // TODO: 実際の敵リストを取得
    // ここでは仮にロックオンシステムからロックオンしている敵を取得
    if (!lockOnSystem_) {
        return;
    }
    
    const auto &lockedEnemies = lockOnSystem_->GetLockedEnemies();
    
    // カメラの方向ベクトルを取得
    Vector3 cameraForward = followCamera_->GetForwardDirection();
    Vector3 cameraRight = followCamera_->GetRightDirection();
    Vector3 cameraUp = followCamera_->GetUpDirection();
    
    // 各敵に対してマーカーを描画
    for (auto *enemy : lockedEnemies) {
        if (!enemy) continue;
        
        // 敵の位置を取得
        Vector3 enemyPos = enemy->GetPosition();
        
        // 敵のタイプを取得（ここでは仮に全て敵と仮定）
        // 実際の実装では敵タイプに応じて処理を分ける
        // enum class EnemyType { Enemy, Friendly, Unknown } などを定義すると良い
        
        // 敵（赤い四角）
        DrawFacingSquare(enemyPos, iffMarkerSize_, enemyMarkerColor_, cameraRight, cameraUp);
        
        // TODO: 友好的ユニット（青い菱形）と未確認ユニット（黄色い円）の描画
        // ここでは省略
    }
}

///=============================================================================
/// 武装インジケーターの描画
void Hud::DrawWeaponIndicator() {
    // 弾薬表示
    Vector2 ammoPos = weaponIndicatorPosition_;
    
    // 「AMMO」ラベル（点線で表現）
    Vector2 ammoLabelPos = {ammoPos.x, ammoPos.y + 15.0f};
    DrawDottedLine(
        {ammoLabelPos.x - 20.0f, ammoLabelPos.y},
        {ammoLabelPos.x + 20.0f, ammoLabelPos.y},
        activeWeaponColor_,
        5.0f,
        2.0f
    );
    
    // 弾薬ブロックを横一列に表示
    float blockX = ammoPos.x - ((maxAmmo_ * (weaponBlockSize_ + weaponBlockSpacing_)) / 2.0f);
    for (int i = 0; i < maxAmmo_; i++) {
        Vector4 blockColor = (i < currentAmmo_) ? activeWeaponColor_ : inactiveWeaponColor_;
        
        // 四角形のブロックを描画
        Vector2 blockPos = {blockX + i * (weaponBlockSize_ + weaponBlockSpacing_), ammoPos.y};
        
        Vector2 topLeft = {blockPos.x - weaponBlockSize_/2, blockPos.y + weaponBlockSize_/2};
        Vector2 topRight = {blockPos.x + weaponBlockSize_/2, blockPos.y + weaponBlockSize_/2};
        Vector2 bottomLeft = {blockPos.x - weaponBlockSize_/2, blockPos.y - weaponBlockSize_/2};
        Vector2 bottomRight = {blockPos.x + weaponBlockSize_/2, blockPos.y - weaponBlockSize_/2};
        
        lineManager_->DrawLine({topLeft.x, topLeft.y, 0}, {topRight.x, topRight.y, 0}, blockColor);
        lineManager_->DrawLine({topRight.x, topRight.y, 0}, {bottomRight.x, bottomRight.y, 0}, blockColor);
        lineManager_->DrawLine({bottomRight.x, bottomRight.y, 0}, {bottomLeft.x, bottomLeft.y, 0}, blockColor);
        lineManager_->DrawLine({bottomLeft.x, bottomLeft.y, 0}, {topLeft.x, topLeft.y, 0}, blockColor);
    }
    
    // ミサイル表示
    Vector2 missilePos = {weaponIndicatorPosition_.x, weaponIndicatorPosition_.y - 30.0f};
    
    // 「MISSILE」ラベル（点線で表現）
    Vector2 missileLabelPos = {missilePos.x, missilePos.y + 15.0f};
    DrawDottedLine(
        {missileLabelPos.x - 30.0f, missileLabelPos.y},
        {missileLabelPos.x + 30.0f, missileLabelPos.y},
        activeWeaponColor_,
        5.0f,
        2.0f
    );
    
    // ミサイルブロックを横一列に表示
    float missileBlockX = missilePos.x - ((maxMissiles_ * (weaponBlockSize_ + weaponBlockSpacing_)) / 2.0f);
    for (int i = 0; i < maxMissiles_; i++) {
        Vector4 blockColor = (i < currentMissiles_) ? activeWeaponColor_ : inactiveWeaponColor_;
        
        // 三角形（ミサイル形状）を描画
        Vector2 blockPos = {missileBlockX + i * (weaponBlockSize_ + weaponBlockSpacing_), missilePos.y};
        
        Vector2 tip = {blockPos.x, blockPos.y + weaponBlockSize_/2};
        Vector2 leftBase = {blockPos.x - weaponBlockSize_/2, blockPos.y - weaponBlockSize_/2};
        Vector2 rightBase = {blockPos.x + weaponBlockSize_/2, blockPos.y - weaponBlockSize_/2};
        
        lineManager_->DrawLine({tip.x, tip.y, 0}, {leftBase.x, leftBase.y, 0}, blockColor);
        lineManager_->DrawLine({leftBase.x, leftBase.y, 0}, {rightBase.x, rightBase.y, 0}, blockColor);
        lineManager_->DrawLine({rightBase.x, rightBase.y, 0}, {tip.x, tip.y, 0}, blockColor);
    }
}

///=============================================================================
/// レーダーディスプレイの描画
void Hud::DrawRadarDisplay() {
    // レーダー背景（四角形）
    float radarSize = radarRadius_ * 2.0f;
    Vector2 topLeft = {radarPosition_.x - radarRadius_, radarPosition_.y + radarRadius_};
    Vector2 topRight = {radarPosition_.x + radarRadius_, radarPosition_.y + radarRadius_};
    Vector2 bottomLeft = {radarPosition_.x - radarRadius_, radarPosition_.y - radarRadius_};
    Vector2 bottomRight = {radarPosition_.x + radarRadius_, radarPosition_.y - radarRadius_};
    
    lineManager_->DrawLine({topLeft.x, topLeft.y, 0}, {topRight.x, topRight.y, 0}, radarBackgroundColor_);
    lineManager_->DrawLine({topRight.x, topRight.y, 0}, {bottomRight.x, bottomRight.y, 0}, radarBackgroundColor_);
    lineManager_->DrawLine({bottomRight.x, bottomRight.y, 0}, {bottomLeft.x, bottomLeft.y, 0}, radarBackgroundColor_);
    lineManager_->DrawLine({bottomLeft.x, bottomLeft.y, 0}, {topLeft.x, topLeft.y, 0}, radarBackgroundColor_);
    
    // レーダーグリッド（内側の四角形）
    for (int i = 1; i <= radarCircles_; i++) {
        float gridSize = radarSize * i / (radarCircles_ + 1);
        Vector2 gridTopLeft = {radarPosition_.x - gridSize/2, radarPosition_.y + gridSize/2};
        Vector2 gridTopRight = {radarPosition_.x + gridSize/2, radarPosition_.y + gridSize/2};
        Vector2 gridBottomLeft = {radarPosition_.x - gridSize/2, radarPosition_.y - gridSize/2};
        Vector2 gridBottomRight = {radarPosition_.x + gridSize/2, radarPosition_.y - gridSize/2};
        
        lineManager_->DrawLine({gridTopLeft.x, gridTopLeft.y, 0}, {gridTopRight.x, gridTopRight.y, 0}, radarGridColor_);
        lineManager_->DrawLine({gridTopRight.x, gridTopRight.y, 0}, {gridBottomRight.x, gridBottomRight.y, 0}, radarGridColor_);
        lineManager_->DrawLine({gridBottomRight.x, gridBottomRight.y, 0}, {gridBottomLeft.x, gridBottomLeft.y, 0}, radarGridColor_);
        lineManager_->DrawLine({gridBottomLeft.x, gridBottomLeft.y, 0}, {gridTopLeft.x, gridTopLeft.y, 0}, radarGridColor_);
    }
    
    // レーダー放射線
    for (int i = 0; i < radarLines_; i++) {
        float angle = i * kTwoPi / radarLines_;
        float cosAngle = cosf(angle);
        float sinAngle = sinf(angle);
        
        lineManager_->DrawLine(
            {radarPosition_.x, radarPosition_.y, 0},
            {radarPosition_.x + radarRadius_ * cosAngle, radarPosition_.y + radarRadius_ * sinAngle, 0},
            radarGridColor_
        );
    }
    
    // プレイヤー自身を中央に表示（青い点）
    DrawFilledCircle(radarPosition_, radarDotSize_, radarDotFriendlyColor_);
    
    // ロックオンしている敵をレーダー上に表示
    if (lockOnSystem_) {
        const auto &lockedEnemies = lockOnSystem_->GetLockedEnemies();
        
        for (auto *enemy : lockedEnemies) {
            if (!enemy) continue;
            
            // 敵のローカル座標を計算（プレイヤーからの相対位置）
            Vector3 playerPos = player_->GetPosition();
            Vector3 enemyPos = enemy->GetPosition();
            Vector3 relPos = {
                enemyPos.x - playerPos.x,
                enemyPos.y - playerPos.y,
                enemyPos.z - playerPos.z
            };
            
            // プレイヤーの向きに合わせて座標回転（プレイヤーの前方が常にレーダーの上になるように）
            float playerYaw = player_->GetRotation().y;
            float cosYaw = cosf(playerYaw);
            float sinYaw = sinf(playerYaw);
            
            float rotX = relPos.x * cosYaw - relPos.z * sinYaw;
            float rotZ = relPos.x * sinYaw + relPos.z * cosYaw;
            
            // レーダー上の座標に変換
            float radarX = rotX / radarRange_ * radarRadius_;
            float radarZ = rotZ / radarRange_ * radarRadius_;
            
            // レーダー範囲内に収める
            if (radarX * radarX + radarZ * radarZ > radarRadius_ * radarRadius_) {
                float angle = atan2f(radarZ, radarX);
                radarX = cosf(angle) * radarRadius_;
                radarZ = sinf(angle) * radarRadius_;
            }
            
            // レーダー上に敵の位置を描画（赤い点）
            Vector2 radarEnemyPos = {
                radarPosition_.x + radarX,
                radarPosition_.y + radarZ
            };
            
            DrawFilledCircle(radarEnemyPos, radarDotSize_, radarDotEnemyColor_);
        }
    }
}

// 塗りつぶし円を描画するヘルパー関数
void Hud::DrawFilledCircle(const Vector2& center, float radius, const Vector4& color) {
    const int segments = 12; // 小さい円なので少ない分割数で十分
    
    // 円周の点を描画
    for (int i = 0; i < segments; i++) {
        float angle1 = i * kTwoPi / segments;
        float angle2 = (i + 1) * kTwoPi / segments;
        
        Vector2 p1 = {
            center.x + radius * cosf(angle1),
            center.y + radius * sinf(angle1)
        };
        
        Vector2 p2 = {
            center.x + radius * cosf(angle2),
            center.y + radius * sinf(angle2)
        };
        
        // 中心から円周への線分を描画（塗りつぶし効果）
        lineManager_->DrawLine({center.x, center.y, 0}, {p1.x, p1.y, 0}, color);
        lineManager_->DrawLine({p1.x, p1.y, 0}, {p2.x, p2.y, 0}, color);
    }
}

///=============================================================================
/// 対地警報の描画
void Hud::DrawGroundWarning() {
    // 対地警報がアクティブでない場合は描画しない
    if (!isGroundWarning_) {
        return;
    }
    
    // 警告表示を点滅させる
    groundWarningTimer_ += 0.016f; // 仮の時間増分
    if (groundWarningTimer_ > groundWarningBlinkRate_) {
        groundWarningTimer_ -= groundWarningBlinkRate_;
    }
    
    // 点滅中の非表示期間の場合は描画しない
    if (groundWarningTimer_ > groundWarningBlinkRate_ * 0.5f) {
        return;
    }
    
    // 画面下部に警告表示
    float halfWidth = 400.0f; // 画面幅の半分（調整要）
    float warningY = -250.0f; // 画面下部の位置
    
    // 波状のパターンを描画
    const int waveSegments = 20;
    const float waveWidth = halfWidth * 1.5f;
    const float waveAmplitude = 10.0f;
    const float waveFrequency = 0.1f;
    
    for (int i = 0; i < waveSegments; i++) {
        float x1 = -waveWidth/2 + i * (waveWidth / waveSegments);
        float x2 = -waveWidth/2 + (i + 1) * (waveWidth / waveSegments);
        
        float y1 = warningY + waveAmplitude * sinf(x1 * waveFrequency * kTwoPi);
        float y2 = warningY + waveAmplitude * sinf(x2 * waveFrequency * kTwoPi);
        
        lineManager_->DrawLine({x1, y1, 0}, {x2, y2, 0}, groundWarningColor_);
    }
    
    // 「PULL UP」警告テキスト（点線で表現）
    Vector2 textPos = {0.0f, warningY - 20.0f};
    DrawDottedLine(
        {textPos.x - 60.0f, textPos.y},
        {textPos.x + 60.0f, textPos.y},
        groundWarningColor_,
        10.0f,
        5.0f
    );
}

///=============================================================================
/// 半透明グリッドラインの描画
void Hud::DrawGridLines() {
    float halfSize = gridSize_ / 2.0f;
    float spacing = gridSize_ / gridLines_;
    
    // 水平グリッド線
    for (int i = 0; i <= gridLines_; i++) {
        float y = -halfSize + i * spacing;
        lineManager_->DrawLine({-halfSize, y, 0}, {halfSize, y, 0}, gridColor_);
    }
    
    // 垂直グリッド線
    for (int i = 0; i <= gridLines_; i++) {
        float x = -halfSize + i * spacing;
        lineManager_->DrawLine({x, -halfSize, 0}, {x, halfSize, 0}, gridColor_);
    }
}

