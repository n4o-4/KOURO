#pragma once
#include "LineManager.h"
#include "FollowCamera.h"
#include "Player.h"
#include "LockOn.h"
#include "Matrixs.h"
#include "MyMath.h"

///=============================================================================
///						Hudクラス
class Hud {
public:
    ///=============================================================================
    ///                        メンバ関数
    // 初期化
    void Initialize(FollowCamera* followCamera, Player* player, LockOn* lockOnSystem);
    
    // 更新
    void Update();
    
    // 描画
    void Draw(ViewProjection viewProjection);
    
    // 戦闘モード切り替え
    void CombatModeActive();
    void CombatModeDeactive();
    
    // ミサイル警報フラグ設定
    void SetMissileWarning(bool isWarning) { isMissileWarning_ = isWarning; }
    
    // 対地警報フラグ設定
    void SetGroundWarning(bool isWarning) { isGroundWarning_ = isWarning; }
    
    ///=============================================================================
    ///                        静的メンバ関数
private:
    // 照準の描画
    void DrawCrosshair(ViewProjection viewProjection);
    
    // ロックオンの描画
    void DrawLockOn(ViewProjection viewProjection);
    
    // カメラに正対する円を描画する補助関数
    void DrawFacingCircle(const Vector3& center, float radius, const Vector4& color, int segments, const Vector3& cameraForward);
    
    // カメラに正対する四角形を描画する補助関数
    void DrawFacingSquare(const Vector3& center, float size, const Vector4& color, const Vector3& cameraRight, const Vector3& cameraUp);
    
    // カメラに正対する進行度円弧を描画する補助関数
    void DrawFacingProgressArc(const Vector3& center, float radius, float startAngle, float endAngle, 
                               const Vector4& color, int segments, const Vector3& cameraRight, const Vector3& cameraUp);
    
    // === 新機能：基本情報パネル ===
    // 速度メーターの描画
    void DrawSpeedIndicator();
    
    // 高度メーターの描画
    void DrawAltitudeIndicator();
    
    // 人工水平線の描画
    void DrawArtificialHorizon();
    
    // Gメーターの描画
    void DrawGForceIndicator();
    
    // === 新機能：戦闘情報パネル ===
    // ミサイル警報の描画
    void DrawMissileWarning();
    
    // 敵機の表示（IFFシステム）
    void DrawIFFSystem(ViewProjection viewProjection);
    
    // 武装インジケーターの描画
    void DrawWeaponIndicator();
    
    // === 新機能：空間認識補助 ===
    // レーダーディスプレイの描画
    void DrawRadarDisplay();
    
    // 対地警報の描画
    void DrawGroundWarning();
    
    // 半透明グリッドラインの描画
    void DrawGridLines();
    
    // ヘルパー関数
    void DrawBar(const Vector2& position, float width, float height, float fillAmount, const Vector4& color);
    void DrawDottedLine(const Vector2& start, const Vector2& end, const Vector4& color, float dashLength, float gapLength);
    void DrawArc(const Vector2& center, float radius, float startAngle, float endAngle, const Vector4& color, int segments);
    // 警告三角形の描画
    void DrawWarningTriangle(const Vector2& center, float size, float rotation, const Vector4& color);
    
    // 塗りつぶし円を描画
    void DrawFilledCircle(const Vector2& center, float radius, const Vector4& color);
        
    
    ///=============================================================================
    ///                        メンバ変数
private:
    // 各種ポインタ
    FollowCamera* followCamera_ = nullptr;
    // プレイヤー
    Player* player_ = nullptr;
    // ロックオンシステム
    LockOn* lockOnSystem_ = nullptr;
    // ラインマネージャ
    LineManager* lineManager_ = nullptr;
    // 敵のリスト
    std::vector<BaseEnemy*> enemies_;
    
    // 数学定数
    static constexpr float kPi = 3.14159f;
    static constexpr float kTwoPi = 2.0f * kPi;
    
    // 戦闘モード
    bool isCombatMode_ = false;
    
    // 照準関連
    float crosshairSize_ = 2.0f;                        // 照準の基本サイズ
    Vector4 crosshairColor_ = {0.0f, 1.0f, 0.0f, 1.0f}; // 照準色（緑）
    float crosshairDistance_ = 20.0f;                   // プレイヤーから照準までの距離
    float crosshairGap_ = 5.0f;                         // 中心からのギャップ
    float crosshairCenterRadius_ = 2.0f;                // 中央の円の半径
    int crosshairCircleSegments_ = 16;                  // 中央の円の分割数
    
    // ロックオン関連
    float lockOnSize_ = 4.0f;                              // ロックオン表示の基本サイズ
    Vector4 lockOnColor_ = {0.0f, 0.0f, 1.0f, 1.0f};        // 基本色（青）
    
    // ロックオンのタイプ別設定
    Vector4 preciseLockonColor_ = {1.0f, 0.0f, 0.0f, 1.0f}; // 精密ロックオン色（赤）
    Vector4 simpleLockonColor_ = {1.0f, 1.0f, 0.0f, 1.0f};  // 簡易ロックオン色（黄）
    Vector4 noLockonColor_ = {0.5f, 0.5f, 0.5f, 0.7f};      // ロックオンなし色（グレー）
    Vector4 progressColor_ = {0.0f, 1.0f, 1.0f, 1.0f};      // 進捗表示色（水色）
    
    float preciseLockonSizeRatio_ = 0.8f;    // 精密ロックオンのサイズ倍率
    float preciseLockonInnerRatio_ = 0.8f;   // 精密ロックオン内側円の比率
    float simpleLockonSizeRatio_ = 0.8f;     // 簡易ロックオンの矩形サイズ倍率
    float noLockonSizeRatio_ = 0.5f;         // ロックオンなしの円サイズ倍率
    
    // ロックオン回転
    float lockOnRotation_ = 0.0f;          // 回転角度
    float lockOnRotationSpeed_ = 0.03f;    // 回転速度
    
    // 進行度インジケーター関連
    int progressArcSegments_ = 32;           // 円弧の分割数
    float progressRadiusRatio_ = 1.7f;       // 進行度インジケーターの半径倍率
    float progressStartAngle_ = -kPi * 0.5f; // 進捗表示開始角度（上から）

    // === 新機能：基本情報パネル ===
    // 速度メーター関連
    Vector2 speedIndicatorPosition_ = {-380.0f, 0.0f}; // 画面左側
    float speedIndicatorWidth_ = 5.0f;
    float speedIndicatorHeight_ = 100.0f;
    Vector4 speedIndicatorColor_ = {0.0f, 1.0f, 0.3f, 1.0f}; // 緑色
    Vector4 speedBackgroundColor_ = {0.2f, 0.2f, 0.2f, 0.5f}; // 背景色
    float maxSpeed_ = 100.0f; // 最大速度（この値を調整）
    
    // 高度メーター関連
    Vector2 altitudeIndicatorPosition_ = {380.0f, 0.0f}; // 画面右側
    float altitudeIndicatorWidth_ = 5.0f;
    float altitudeIndicatorHeight_ = 100.0f;
    Vector4 altitudeIndicatorColor_ = {0.0f, 0.7f, 1.0f, 1.0f}; // 水色
    Vector4 altitudeBackgroundColor_ = {0.2f, 0.2f, 0.2f, 0.5f}; // 背景色
    float maxAltitude_ = 300.0f; // 最大高度（この値を調整）
    
    // 人工水平線関連
    float horizonLength_ = 150.0f;
    float horizonThickness_ = 2.0f;
    Vector4 horizonColor_ = {1.0f, 1.0f, 1.0f, 0.7f}; // 白色（半透明）
    Vector4 horizonReferenceColor_ = {0.5f, 0.5f, 0.5f, 0.3f}; // 参照線（薄灰色）
    int horizonSteps_ = 5;  // 水平線の上下の参照線数
    float horizonStepSize_ = 15.0f; // 参照線の間隔
    
    // Gメーター関連
    Vector2 gForcePosition_ = {0.0f, -250.0f}; // 画面下部中央
    float gForceRadius_ = 50.0f;
    Vector4 gForceColor_ = {1.0f, 0.5f, 0.0f, 1.0f}; // オレンジ
    Vector4 gForceBackgroundColor_ = {0.2f, 0.2f, 0.2f, 0.5f}; // 背景色
    float maxGForce_ = 10.0f; // 最大G値
    float currentGForce_ = 0.0f; // 現在のG値
    float gForceSmoothingFactor_ = 0.1f; // G値平滑化係数
    
    // === 新機能：戦闘情報パネル ===
    // ミサイル警報関連
    Vector4 missileWarningColor_ = {1.0f, 0.0f, 0.0f, 1.0f}; // 赤色
    float missileWarningSize_ = 40.0f;
    bool isMissileWarning_ = false;
    float missileWarningBlinkRate_ = 0.5f; // 点滅速度
    float missileWarningTimer_ = 0.0f;
    
    // IFFシステム関連
    float iffMarkerSize_ = 3.0f;
    Vector4 enemyMarkerColor_ = {1.0f, 0.0f, 0.0f, 1.0f}; // 敵：赤
    Vector4 friendlyMarkerColor_ = {0.0f, 0.0f, 1.0f, 1.0f}; // 味方：青
    Vector4 unknownMarkerColor_ = {1.0f, 1.0f, 0.0f, 1.0f}; // 未確認：黄
    
    // 武装インジケーター関連
    Vector2 weaponIndicatorPosition_ = {-320.0f, -250.0f}; // 画面左下
    float weaponBlockSize_ = 10.0f;
    float weaponBlockSpacing_ = 2.0f;
    Vector4 activeWeaponColor_ = {0.0f, 1.0f, 0.0f, 1.0f}; // 緑
    Vector4 inactiveWeaponColor_ = {0.3f, 0.3f, 0.3f, 0.5f}; // グレー
    int maxAmmo_ = 20; // 最大弾薬数
    int currentAmmo_ = 15; // 現在の弾薬数
    int maxMissiles_ = 6; // 最大ミサイル数
    int currentMissiles_ = 4; // 現在のミサイル数
    
    // === 新機能：空間認識補助 ===
    // レーダーディスプレイ関連
    Vector2 radarPosition_ = {320.0f, -250.0f}; // 画面右下
    float radarRadius_ = 60.0f;
    Vector4 radarBackgroundColor_ = {0.0f, 0.2f, 0.0f, 0.5f}; // 暗い緑（半透明）
    Vector4 radarGridColor_ = {0.0f, 0.8f, 0.0f, 0.3f}; // 明るい緑（薄い）
    Vector4 radarDotEnemyColor_ = {1.0f, 0.0f, 0.0f, 1.0f}; // 敵：赤
    Vector4 radarDotFriendlyColor_ = {0.0f, 0.0f, 1.0f, 1.0f}; // 味方：青
    float radarDotSize_ = 3.0f;
    float radarRange_ = 500.0f; // レーダー検出範囲
    int radarCircles_ = 3; // レーダーの円の数
    int radarLines_ = 8; // レーダーの放射線の数
    
    // 対地警報関連
    Vector4 groundWarningColor_ = {1.0f, 0.5f, 0.0f, 1.0f}; // オレンジ
    float groundWarningHeight_ = 30.0f; // 警報を出す高度
    bool isGroundWarning_ = false;
    float groundWarningBlinkRate_ = 0.3f; // 点滅速度
    float groundWarningTimer_ = 0.0f;
    
    // グリッドライン関連
    Vector4 gridColor_ = {0.2f, 0.6f, 0.8f, 0.15f}; // 水色（半透明）
    int gridLines_ = 20; // グリッド線の数
    float gridSize_ = 800.0f; // グリッドの大きさ
};