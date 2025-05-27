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

	///=============================================================================
	///                        静的メンバ関数
private:
	// 照準の描画
	void DrawCrosshair(ViewProjection viewProjection);

	// ロックオンの描画
	void DrawLockOn(ViewProjection viewProjection);

	// レーダーディスプレイの描画
	void DrawRadarDisplay(ViewProjection viewProjection);

	// 3D球体ミニマップの描画 - このメソッドを追加
	void DrawSphereMap(ViewProjection viewProjection);

	// プレイヤーステータスバーの描画
	void DrawPlayerStatusBars(ViewProjection viewProjection);

	// カメラに正対する円を描画する補助関数
	void DrawFacingCircle(const Vector3 &center, float radius, const Vector4 &color, int segments, const Vector3 &cameraForward);

	// カメラに正対する四角形を描画する補助関数
	void DrawFacingSquare(const Vector3 &center, float size, const Vector4 &color, const Vector3 &cameraRight, const Vector3 &cameraUp);

	// カメラに正対する進行度円弧を描画する補助関数
	void DrawFacingProgressArc(const Vector3 &center, float radius, float startAngle, float endAngle,
							   const Vector4 &color, int segments, const Vector3 &cameraRight, const Vector3 &cameraUp);

	// カメラに正対する多角形を描画する補助関数
	void DrawFacingPolygon(const Vector3 &center, float size, int segments, const Vector4 &color, const Vector3 &cameraForward);

	// カメラに正対する三角形を描画する補助関数
	void DrawFacingTriangle(const Vector3 &center, float size, const Vector4 &color, const Vector3 &cameraForward);

	// 3D球体ミニマップ関連の関数
	void DrawSphereGrid(const Vector3 &center, float radius, const Vector4 &color, const Vector3 &cameraForward);
	Vector3 WorldToSpherePosition(const Vector3 &worldPos, const Vector3 &sphereCenter, float radius, float maxRange);
	void Draw3DTriangle(const Vector3 &center, float size, const Vector4 &color, const Vector3 &direction);
	void Draw3DHexagon(const Vector3 &center, float size, const Vector4 &color, const Vector3 &direction);

	///=============================================================================
	///                        メンバ変数
private:
	// 各種ポインタ
	FollowCamera *followCamera_ = nullptr;
	// プレイヤー
	Player *player_ = nullptr;
	// ロックオンシステム
	LockOn *lockOnSystem_ = nullptr;
	// ラインマネージャ
	LineManager *lineManager_ = nullptr;
	// 敵のリスト
	const std::vector<std::unique_ptr<BaseEnemy>> *enemies_ = nullptr;
	// スポーンのリスト
	const std::vector<std::unique_ptr<BaseEnemy>> *spawns_ = nullptr;

	// 数学定数
	static constexpr float kPi = 3.14159f;
	static constexpr float kTwoPi = 2.0f * kPi;

	// 戦闘モード
	bool isCombatMode_ = false;

	// HUDの基本色設定
	Vector4 hudBaseColor_ = {0.0f, 1.0f, 0.4f, 0.9f};	// 基本色（緑）
	Vector4 hudAccentColor_ = {0.2f, 1.0f, 0.8f, 0.9f}; // アクセント色（シアン系）
	Vector4 hudEnemyColor_ = {1.0f, 0.3f, 0.0f, 0.9f};	// 敵標識色（赤橙）
	Vector4 hudAlertColor_ = {1.0f, 0.8f, 0.0f, 0.9f};	// 警告色（黄色）
	Vector4 hudLockColor_ = {1.0f, 0.0f, 0.0f, 0.9f};	// ロック完了色（赤）

	// 照準関連
	float crosshairSize_ = 2.5f;			 // 照準の基本サイズ
	Vector4 crosshairColor_ = hudBaseColor_; // 照準色を基本色に統一
	float crosshairDistance_ = 64.0f;		 // プレイヤーから照準までの距離
	float crosshairGap_ = 8.0f;				 // 中心からのギャップを拡大
	float crosshairCenterRadius_ = 0.8f;	 // 中央の円の半径を小さく
	int crosshairCircleSegments_ = 16;		 // 中央の円の分割数

	// ロックオン関連
	float lockOnSize_ = 5.0f;			  // ロックオン表示の基本サイズ
	Vector4 lockOnColor_ = hudBaseColor_; // 基本色（緑）

	// ロックオンのタイプ別設定
	Vector4 preciseLockonColor_ = hudLockColor_;	   // 精密ロックオン色（赤）
	Vector4 simpleLockonColor_ = hudAlertColor_;	   // 簡易ロックオン色（黄）
	Vector4 noLockonColor_ = {0.5f, 0.8f, 0.5f, 0.7f}; // ロックオンなし色（薄緑）
	Vector4 progressColor_ = hudAccentColor_;		   // 進捗表示色（シアン）

	float preciseLockonSizeRatio_ = 0.6f;  // 精密ロックオンのサイズ倍率
	float preciseLockonInnerRatio_ = 0.2f; // 精密ロックオン内側円の比率
	float simpleLockonSizeRatio_ = 0.4f;   // 簡易ロックオンの矩形サイズ倍率
	float noLockonSizeRatio_ = 0.2f;	   // ロックオンなしの円サイズ倍率

	// ロックオン回転
	float lockOnRotation_ = 0.0f;		// 回転角度
	float lockOnRotationSpeed_ = 0.02f; // 回転速度（少し遅く）

	// ミニマップ表示関連
	bool showMiniMap_ = true;				   // ミニマップを表示するか
	Vector4 enemyDotColor_ = hudEnemyColor_;   // 敵の色
	Vector4 spawnBlockColor_ = hudAlertColor_; // スポーンブロックの色

	// レーダー表示関連（追加）
	bool showRadar_ = true;			// レーダーを表示するか
	bool rotateWithCamera_ = false; // カメラの向きに合わせてレーダーを回転するか
	bool use3DSphereMap_ = true;	// 3D球体ミニマップを使用するか
	bool useCircularRadar_ = true;	// 円形レーダーを使用するか

	// レーダーの表示設定（追加）
	float radarSize_ = 5.0f;								// レーダーのサイズ
	float radarPositionX_ = -15.0f;							// レーダーのX座標位置
	float radarPositionY_ = 10.0f;							// レーダーのY座標位置
	float radarRange_ = 100.0f;								// レーダーの検出範囲
	float radarRotation_ = 0.0f;							// レーダーの回転角
	float radarInnerCircleRatio_ = 0.5f;					// 内側の円の比率
	int radarCircleSegments_ = 32;							// 円の分割数
	float radarSweepSpeed_ = 0.03f;							// スイープの速度
	float radarSweepAngle_ = 0.0f;							// スイープの角度
	bool showRadarSweep_ = true;							// スイープを表示するか
	Vector4 radarBorderColor_ = {0.0f, 1.0f, 0.3f, 0.7f};	// レーダーの枠線の色
	Vector4 radarGridColor_ = {0.0f, 1.0f, 0.3f, 0.4f};		// レーダーのグリッドの色
	Vector4 radarSweepColor_ = {0.0f, 1.0f, 0.3f, 0.9f};	// レーダーのスイープの色
	Vector4 radiationLineColor_ = {0.0f, 1.0f, 0.3f, 0.3f}; // 放射線の色
	float enemyDotSize_ = 0.4f;								// 敵のドットサイズ
	float spawnBlockSize_ = 0.2f;							// スポーンブロックのサイズ

	// 3D球体ミニマップ関連
	float sphereMapRadius_ = 6.0f;							// 球体の半径を適切に設定
	float sphereMapPositionX_ = 10.0f;						// 右側に配置（値を大きく）
	float sphereMapPositionY_ = -6.0f;						// 上側に配置（値を大きく）
	float sphereMapPositionZ_ = 27.0f;						// Z軸は0に設定
	float sphereMapRange_ = 512.0f;							// 検出範囲
	int sphereLongitudeCount_ = 6;							// 経線の数
	int sphereLatitudeCount_ = 3;							// 緯線の数
	float sphereRotationSpeed_ = 0.000f;					// 回転速度
	float sphereRotationY_ = 0.0f;							// Y軸周りの回転
	float sphereObjectScale_ = 0.4f;						// オブジェクトのスケール
	Vector4 sphereMapColor_ = {0.0f, 0.8f, 0.4f, 0.7f};		// 球体マップの色を追加
	Vector4 sphereMapLockColor_ = {1.0f, 0.2f, 0.2f, 0.7f}; // ロック時の色を追加
	float sphereMapFov_ = 60.0f * (kPi / 180.0f);			// 視界の角度 (デフォルト60度)
	Vector4 sphereMapFovColor_ = {0.5f, 0.8f, 1.0f, 0.3f};	// 視界扇形の色

	// プレイヤーステータスバー関連
	float statusBarWidth_ = 10.0f;							   // ステータスバーの基本幅
	float statusBarHeight_ = 0.5f;							   // ステータスバーの高さ
	float boostBarPositionX_ = 0.0f;						   // ブーストバーのX位置（画面中央からのオフセット）
	float boostBarPositionY_ = -12.0f;						   // ブーストバーのY位置（画面下部からのオフセット）
	float weaponBarPositionX_ = 8.0f;						   // 武器バーのX位置（照準からのオフセット）
	float weaponBarPositionY_ = -5.0f;						   // 武器バーのY位置（照準からのオフセット）
	Vector4 boostGaugeColor_ = {0.2f, 0.8f, 1.0f, 0.8f};	   // ブーストゲージの色
	Vector4 quickBoostChargeColor_ = {1.0f, 1.0f, 0.2f, 0.9f}; // クイックブーストチャージの色
	Vector4 weaponReadyColor_ = {0.2f, 1.0f, 0.5f, 0.8f};	   // 武器準備完了時の色
	Vector4 weaponReloadColor_ = {1.0f, 0.6f, 0.2f, 0.7f};	   // 武器リロード中の色
	Vector4 overheatColor_ = {1.0f, 0.2f, 0.1f, 0.9f};		   // オーバーヒート時の色
};