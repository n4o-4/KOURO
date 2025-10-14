#include "PrimitiveDrawer.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct ScanEffectCB
{
    float progress;
    //float3 scanColor; // スキャン線の色
    //float3 baseColor; // モデルの基本色
    float scanThickness; // スキャン線の厚み（任意）
    int isRenderScanned; // スキャン済みラインを描画するかどうかのフラグ（0または1）
};

ConstantBuffer<ScanEffectCB> gScanEffectCB : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    //PixelShaderOutput output;
    
    //output.color.rgb = input.color.rgb;
    
    //output.color.a = 1.0f;
    
    //return output;
    PixelShaderOutput output;

    float viewportHeight = 720.0f; // 例
    float yScreen = input.position.y / viewportHeight;

    float scanPos = gScanEffectCB.progress;
    float thickness = gScanEffectCB.scanThickness;

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
    return output;
}