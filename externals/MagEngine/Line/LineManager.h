#pragma once
#include <vector>
#include <memory>
#include "Line.h"
#include "LineSetup.h"
#include "DirectXCommon.h"
#include "SrvManager.h"
#include "ViewProjection.h"

/// ラインマネージャクラス
class LineManager {
public:
    /**
     * インスタンスの取得
     */
    static LineManager* GetInstance();

    /**
     * 初期化処理
     */
    void Initialize(DirectXCommon* dxCommon, SrvManager* srvManager);

    /**
     * 終了処理
     */
    void Finalize();

    /**
     * 更新処理
     */
    void Update();

    /**
     * 描画処理
     */
    void Draw();

    /**
     * ImGui描画
     */
    void DrawImGui();

    /**
     * ラインのクリア
     */
    void ClearLines();

    /**
     * ラインの描画
     */
    void DrawLine(const Vector3& start, const Vector3& end, const Vector4& color);

    /**
     * グリッドの描画
     */
    void DrawGrid(float gridSize, int divisions, const Vector4& color);

    /**
     * 球体の描画
     */
    void DrawSphere(const Vector3& center, float radius, const Vector4& color, int divisions = 32);
    
    /**
     * 円の描画
     */
    void DrawCircle(const Vector3& center, float radius, const Vector4& color, int divisions = 32);

    /**
     * デフォルトカメラの取得
     */
    ViewProjection* GetDefaultCamera() { return lineSetup_->GetDefaultCamera(); }

    /**
     * デフォルトカメラの設定
     */
    void SetDefaultCamera(ViewProjection* viewProjection) { lineSetup_->SetDefaultCamera(viewProjection); }

private:
    static LineManager* instance_;

    //コンストラクタ
    LineManager() = default;
    //デストラクタ
    ~LineManager() = default;
    //コピーコンストラクタ
    LineManager(const LineManager&) = delete;    
    //代入演算子
    LineManager& operator=(const LineManager&) = delete;

    // DirectXCommonへの参照
    DirectXCommon* dxCommon_ = nullptr;
    // SrvManagerへの参照
    SrvManager* srvManager_ = nullptr;

    // Line
    std::unique_ptr<Line> line_;

    // LineSetupインスタンス
    std::unique_ptr<LineSetup> lineSetup_;

    // 描画設定
    bool isDrawLine_ = true;   // ラインを描画するか
    bool isDrawGrid_ = true;   // グリッドを描画するか
    float gridSize_ = 64.0f;   // グリッドサイズ
    int gridDivisions_ = 8;    // グリッド分割数
    Vector4 gridColor_ = { 0.5f, 0.5f, 0.5f, 1.0f };  // グリッド色

    // 球体を描画するか
    bool isDrawSphere_ = true;
};