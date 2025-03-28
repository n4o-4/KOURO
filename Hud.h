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
};