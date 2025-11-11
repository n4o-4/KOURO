#include "PrimitiveDrawer.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct ScanEffectCB
{
    bool enableScan; // スキャンエフェクトを有効にするかどうか
    bool isRenderScanned; // スキャン部分を描画するかどうか
    float progress; // 0.0f～1.0f スキャンの進行度
    float thickness; // スキャンの厚み
};

ConstantBuffer<ScanEffectCB> gScanEffectCB : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    
    PixelShaderOutput output;
    
    if(gScanEffectCB.enableScan == 0)
    {
        output.color.rgb = input.color.rgb;
    
        output.color.a = 1.0f;
    }
    else
    {
        float viewportHeight = 720.0f; // 例
        float yScreen = input.position.y / viewportHeight;

        float scanPos = gScanEffectCB.progress;
        float thickness = gScanEffectCB.thickness;

    // スキャン中心からの距離
        float dist = abs(yScreen - scanPos);

    // 中心が1（青）、端が0（白）になるマスク
        float lineMask = saturate(1.0 - dist / thickness);

    // スキャン済みマスク（0または1）
        float scannedMask = step(0.0f, scanPos - yScreen);

        int renderScanned = 0; // スキャン済みラインを描画するかどうかのフラグ（0または1）
    
        if (gScanEffectCB.isRenderScanned == 0)
        {
            if (!scannedMask)
            {
                discard;
            }
        }
        else
        {
        
        }
   
    
    // 色設定
        float3 scanColor = float3(0.0, 0.451, 0.871); // スキャン中ラインの青
        float3 scannedColor = float3(1, 1, 1); // スキャン済み白

        float3 color = scannedColor;

    //// スキャン済みラインはフラグが true の場合のみ描画
    //if (renderScanned)
    //{
    //    color = lerp(color, scannedColor, scannedMask);
    //}

    // スキャン中ラインを中心青・端白のグラデーション
        color = lerp(color, scanColor, lineMask);

        output.color = float4(color, 1.0f);
    }
   
    return output;
}