#pragma once
#include "FollowCamera.h"
#include "LineManager.h"
#include "LockOn.h"
#include "Matrixs.h"
#include "MyMath.h"
#include "Player.h"

///=============================================================================
///						Hudクラス
class Hud {
public:
	///=============================================================================
	///                        メンバ関数
	// 初期化
	void Initialize(FollowCamera *followCamera, Player *player, LockOn *lockOnSystem);

	// 更新
	void Update();

	// 描画
	void Draw(ViewProjection viewProjection);

	void DrawImGui();

	// 戦闘モード切り替え
	void CombatModeActive();
	void CombatModeDeactive();

	// 敵コンテナとスポーンブロックの設定
	void SetEnemiesAndSpawns(const std::vector<std::unique_ptr<BaseEnemy>> *enemies,
							 const std::vector<std::unique_ptr<BaseEnemy>> *spawns);

private:
	// 照準の描画
	void DrawCrosshair(ViewProjection viewProjection);

	// ロックオンの描画
	void DrawLockOn(ViewProjection viewProjection);

	// 3D球体ミニマップの描画
	void DrawSphereMap(ViewProjection viewProjection);

	// プレイヤーステータスバーの描画
	void DrawPlayerStatusBars(ViewProjection viewProjection);

	// カメラに正対する円を描画する補助関数
	void DrawFacingCircle(const Vector3 &center, float radius, const Vector4 &color, int segments, const Vector3 &cameraForward);

	// カメラに正対する四角形を描画する補助関数
	void DrawFacingSquare(const Vector3 &center, float size, const Vector4 &color, const Vector3 &cameraRight, const Vector3 &cameraUp);

	// カメラに正対する多角形を描画する補助関数
	void DrawFacingPolygon(const Vector3 &center, float size, int segments, const Vector4 &color, const Vector3 &cameraForward);

	// カメラに正対する三角形を描画する補助関数
	void DrawFacingTriangle(const Vector3 &center, float size, const Vector4 &color, const Vector3 &cameraForward);

	// 2Dステータスバーを描画する補助関数
	void DrawStatusBar(const Vector3 &center, float width, float height, const Vector4 &color,
					   const Vector3 &cameraRight, const Vector3 &cameraUp);

	// ステータスバーの背景枠を描画する補助関数
	void DrawStatusBarFrame(const Vector3 &center, float width, float height, const Vector4 &color,
							const Vector3 &cameraRight, const Vector3 &cameraUp);

	// 3D球体ミニマップ関連の関数
	Vector3 WorldToSpherePosition(const Vector3 &worldPos, const Vector3 &sphereCenter, float radius, float maxRange);
	void Draw3DTriangle(const Vector3 &center, float size, const Vector4 &color, const Vector3 &direction);
	void Draw3DHexagon(const Vector3 &center, float size, const Vector4 &color, const Vector3 &direction);
	void DrawSphereGrid(const Vector3 &center, float radius, const Vector4 &color);

	///=============================================================================
	///                        メンバ変数
private:
	// 各種ポインタ
	FollowCamera *followCamera_ = nullptr;
	Player *player_ = nullptr;
	LockOn *lockOnSystem_ = nullptr;
	LineManager *lineManager_ = nullptr;
	const std::vector<std::unique_ptr<BaseEnemy>> *enemies_ = nullptr;
	const std::vector<std::unique_ptr<BaseEnemy>> *spawns_ = nullptr;

	// 数学定数
	static constexpr float kPi = 3.14159f;
	static constexpr float kTwoPi = 2.0f * kPi;

	// 戦闘モード
	bool isCombatMode_ = false;

	// HUDの基本色設定
	Vector4 hudBaseColor_ = {0.0f, 1.0f, 0.4f, 0.9f};
	Vector4 hudAccentColor_ = {0.2f, 1.0f, 0.8f, 0.9f};
	Vector4 hudEnemyColor_ = {1.0f, 0.3f, 0.0f, 0.9f};
	Vector4 hudAlertColor_ = {1.0f, 0.8f, 0.0f, 0.9f};
	Vector4 hudLockColor_ = {1.0f, 0.0f, 0.0f, 0.9f};

	// 照準関連
	float crosshairSize_ = 2.5f;
	Vector4 crosshairColor_ = hudBaseColor_;
	float crosshairDistance_ = 64.0f;
	float crosshairGap_ = 8.0f;
	float crosshairCenterRadius_ = 0.8f;
	int crosshairCircleSegments_ = 16;

	// ロックオン関連
	float lockOnSize_ = 5.0f;
	Vector4 lockOnColor_ = hudLockColor_;
	Vector4 preciseLockonColor_ = hudLockColor_;
	Vector4 simpleLockonColor_ = hudAlertColor_;
	Vector4 noLockonColor_ = {0.5f, 0.8f, 0.5f, 0.7f};
	Vector4 progressColor_ = hudAccentColor_;
	float preciseLockonSizeRatio_ = 0.6f;
	float simpleLockonSizeRatio_ = 0.4f;
	float noLockonSizeRatio_ = 0.2f;
	float lockOnRotation_ = 0.0f;
	float lockOnRotationSpeed_ = 0.02f;

	// 3D球体ミニマップ関連
	bool use3DSphereMap_ = true;
	float sphereMapRadius_ = 6.0f;
	float sphereMapPositionX_ = 10.0f;
	float sphereMapPositionY_ = -6.0f;
	float sphereMapPositionZ_ = 27.0f;
	float sphereMapRange_ = 512.0f;
	float sphereObjectScale_ = 0.4f;
	Vector4 sphereMapFovColor_ = {0.5f, 0.8f, 1.0f, 0.3f};
	float sphereMapFov_ = 60.0f * (kPi / 180.0f);
	Vector4 enemyDotColor_ = hudEnemyColor_;
	Vector4 spawnBlockColor_ = hudAlertColor_;
	Vector4 sphereGridColor_ = {0.0f, 0.8f, 0.4f, 0.6f};
	int sphereLongitudeCount_ = 8;
	int sphereLatitudeCount_ = 4;

	// プレイヤーステータスバー関連
	float screenDistance_ = 50.0f;
	float statusBarWidth_ = 8.0f;
	float statusBarHeight_ = 0.8f;

	// ブーストゲージ位置
	float boostBarOffsetX_ = -22.0f;
	float boostBarOffsetY_ = -18.0f;
	float quickBoostChargeSpacing_ = 1.5f;
	float quickBoostChargeHeight_ = 0.4f;
	float rechargeSpacing_ = 1.0f;
	float rechargeBarHeight_ = 0.3f;

	// 武器ゲージ位置
	float mgBarOffsetX_ = 8.0f;
	float mgBarOffsetY_ = -18.0f;
	float weaponCooldownSpacing_ = 1.2f;
	float weaponCooldownHeight_ = 0.5f;
	float missileBarSpacing_ = -3.5f;
	float missileReadySpacing_ = 1.0f;
	float missileReadyHeight_ = 0.3f;

	// ゲージ色設定
	Vector4 boostGaugeColor_ = {0.2f, 0.8f, 1.0f, 0.8f};
	Vector4 quickBoostChargeColor_ = {1.0f, 1.0f, 0.2f, 0.9f};
	Vector4 weaponReadyColor_ = {0.2f, 1.0f, 0.5f, 0.8f};
	Vector4 weaponReloadColor_ = {1.0f, 0.6f, 0.2f, 0.7f};
	Vector4 weaponHeatColor_ = {1.0f, 0.8f, 0.0f, 0.8f};
	Vector4 overheatColor_ = {1.0f, 0.2f, 0.1f, 0.9f};
	Vector4 gaugeBorderColor_ = {0.3f, 0.6f, 0.3f, 0.6f};
	float gaugeBorderWidth_ = 0.1f;
};