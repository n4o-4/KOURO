#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSamplerLinear : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    // ���S�_�B���������ɕ��ː���Ƀu���[��������
    const float2 kCenter = float2(0.5f, 0.5f); 
    
    // �T���v�����O���B�����قǊ��炩 (�d���Ȃ�)
    const int kNumSamples = 10; 
    
    // �ڂ����̕��B
    float kBlurWidth = 0.01f;
    
    // ���K��������������艓�����T���v�����O����
    float2 direction = input.texcoord - kCenter;
    float3 outputColor = float3(0.0f, 0.0f, 0.0f);
    for (int sampleIndex = 0; sampleIndex < kNumSamples;++sampleIndex)
    {
        // ���݂�uv�����قǌv�Z���������ɃT���v���_��i�߂Ȃ���T���v�����O���Ă���
        float2 texcoord = input.texcoord + direction * kBlurWidth * float(sampleIndex);

        outputColor.rgb += gTexture.Sample(gSamplerLinear, texcoord).rgb;
    }
    // ���ω�����
    outputColor.rgb *= rcp(kNumSamples);
    
    PixelShaderOutput output;
    output.color.rgb = outputColor;
    output.color.a = 1.0f;
    
    return output;
}