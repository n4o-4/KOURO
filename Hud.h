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
    
    // 敵コンテナとスポーンブロックの設定
    void SetEnemiesAndSpawns(const std::vector<std::unique_ptr<BaseEnemy>>* enemies, 
                             const std::vector<std::unique_ptr<BaseEnemy>>* spawns);
    
    ///=============================================================================
    ///                        静的メンバ関数
private:
    // 照準の描画
    void DrawCrosshair(ViewProjection viewProjection);
    
    // ロックオンの描画
    void DrawLockOn(ViewProjection viewProjection);

    // レーダーディスプレイの描画
    void DrawRadarDisplay(ViewProjection viewProjection);
    
    // レーダー上の点を計算する（ワールド座標からレーダー座標へ変換）
    Vector2 CalculateRadarPosition(const Vector3& worldPosition);
    
    // レーダー上に敵を描画
    void DrawEnemyOnRadar(const Vector3& enemyPosition, const Vector4& color);
    
    // レーダー上にスポーンブロックを描画
    void DrawSpawnOnRadar(const Vector3& spawnPosition, const Vector4& color);
    
    // 自機から敵への放射線を描画
    void DrawRadiationLine(const Vector3& enemyPosition, const Vector4& color);

    // カメラに正対する円を描画する補助関数
    void DrawFacingCircle(const Vector3& center, float radius, const Vector4& color, int segments, const Vector3& cameraForward);
    
    // カメラに正対する四角形を描画する補助関数
    void DrawFacingSquare(const Vector3& center, float size, const Vector4& color, const Vector3& cameraRight, const Vector3& cameraUp);
    
    // カメラに正対する進行度円弧を描画する補助関数
    void DrawFacingProgressArc(const Vector3& center, float radius, float startAngle, float endAngle, 
                               const Vector4& color, int segments, const Vector3& cameraRight, const Vector3& cameraUp);

                               // カメラに正対する多角形を描画する補助関数
void DrawFacingPolygon(const Vector3& center, float size, int segments, const Vector4& color, const Vector3& cameraForward);

// カメラに正対する三角形を描画する補助関数
void DrawFacingTriangle(const Vector3& center, float size, const Vector4& color, const Vector3& cameraForward);
    
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
    const std::vector<std::unique_ptr<BaseEnemy>>* enemies_ = nullptr;
    // スポーンのリスト
    const std::vector<std::unique_ptr<BaseEnemy>>* spawns_ = nullptr;
    
    // 数学定数
    static constexpr float kPi = 3.14159f;
    static constexpr float kTwoPi = 2.0f * kPi;
    
    // 戦闘モード
    bool isCombatMode_ = false;
    
    // HUDの基本色設定（エースコンバットスタイル）
    Vector4 hudBaseColor_ = {0.0f, 1.0f, 0.4f, 0.9f};     // 基本色（緑）
    Vector4 hudAccentColor_ = {0.2f, 1.0f, 0.8f, 0.9f};   // アクセント色（シアン系）
    Vector4 hudEnemyColor_ = {1.0f, 0.3f, 0.0f, 0.9f};    // 敵標識色（赤橙）
    Vector4 hudAlertColor_ = {1.0f, 0.8f, 0.0f, 0.9f};    // 警告色（黄色）
    Vector4 hudLockColor_ = {1.0f, 0.0f, 0.0f, 0.9f};     // ロック完了色（赤）
    
    // 照準関連
    float crosshairSize_ = 2.5f;                         // 照準の基本サイズ
    Vector4 crosshairColor_ = hudBaseColor_;             // 照準色を基本色に統一
    float crosshairDistance_ = 20.0f;                    // プレイヤーから照準までの距離
    float crosshairGap_ = 8.0f;                          // 中心からのギャップを拡大
    float crosshairCenterRadius_ = 0.8f;                 // 中央の円の半径を小さく
    int crosshairCircleSegments_ = 16;                   // 中央の円の分割数
    
    // ロックオン関連
    float lockOnSize_ = 5.0f;                           // ロックオン表示の基本サイズ
    Vector4 lockOnColor_ = hudBaseColor_;               // 基本色（緑）
    
    // ロックオンのタイプ別設定
    Vector4 preciseLockonColor_ = hudLockColor_;        // 精密ロックオン色（赤）
    Vector4 simpleLockonColor_ = hudAlertColor_;        // 簡易ロックオン色（黄）
    Vector4 noLockonColor_ = {0.5f, 0.8f, 0.5f, 0.7f};  // ロックオンなし色（薄緑）
    Vector4 progressColor_ = hudAccentColor_;           // 進捗表示色（シアン）
    
    float preciseLockonSizeRatio_ = 0.6f;     // 精密ロックオンのサイズ倍率
    float preciseLockonInnerRatio_ = 0.2f;    // 精密ロックオン内側円の比率
    float simpleLockonSizeRatio_ = 0.4f;      // 簡易ロックオンの矩形サイズ倍率
    float noLockonSizeRatio_ = 0.2f;          // ロックオンなしの円サイズ倍率
    
    // ロックオン回転
    float lockOnRotation_ = 0.0f;          // 回転角度
    float lockOnRotationSpeed_ = 0.02f;    // 回転速度（少し遅く）
    
    // 進行度インジケーター関連
    int progressArcSegments_ = 32;           // 円弧の分割数
    float progressRadiusRatio_ = 1.5f;       // 進行度インジケーターの半径倍率
    float progressStartAngle_ = -kPi * 0.5f; // 進捗表示開始角度（上から）
    
    // レーダーディスプレイ関連（エースコンバットスタイル）
    bool showRadar_ = true;                           // レーダーを表示するか
    float radarSize_ = 14.0f;                         // レーダーのサイズを拡大
    float radarPositionX_ = 14.0f;                    // より右に配置
    float radarPositionY_ = 9.0f;                     // より上に配置
    float radarRange_ = 150.0f;                       // レーダーの検出範囲
    Vector4 radarBorderColor_ = hudBaseColor_;        // レーダー枠の色
    Vector4 radarBackgroundColor_ = {0.0f, 0.3f, 0.1f, 0.2f}; // 暗い緑の背景
    Vector4 radarSweepColor_ = {0.0f, 0.8f, 0.4f, 0.6f};  // レーダースイープ色
    Vector4 radarGridColor_ = {0.0f, 0.6f, 0.3f, 0.3f};   // レーダーグリッド色
    Vector4 enemyDotColor_ = hudEnemyColor_;            // 敵の点の色
    Vector4 spawnBlockColor_ = hudAlertColor_;          // スポーンブロックの色
    Vector4 radiationLineColor_ = {0.0f, 0.9f, 0.5f, 0.3f}; // 放射線の色（薄く）
    float enemyDotSize_ = 0.4f;                      // 敵の点のサイズ
    float spawnBlockSize_ = 0.5f;                    // スポーンブロックのサイズ
    float radiationLineWidth_ = 1.0f;                // 放射線の幅
    float radarRotation_ = 0.0f;                     // レーダーの回転
    bool rotateWithCamera_ = true;                   // カメラの向きに合わせてレーダーを回転させるか
    
    // レーダー円形表示用
    bool useCircularRadar_ = true;               // 円形レーダーを使用
    int radarCircleSegments_ = 32;               // レーダー円の分割数
    float radarInnerCircleRatio_ = 0.6f;         // 内側の円の比率
    float radarSweepSpeed_ = 0.03f;              // スイープ速度
    float radarSweepAngle_ = 0.0f;               // スイープ角度
    bool showRadarSweep_ = true;                 // レーダースイープを表示するか
};