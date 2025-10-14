#include "PrimitiveDrawer.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct ScanEffectCB
{
    float progress;
    //float3 scanColor; // �X�L�������̐F
    //float3 baseColor; // ���f���̊�{�F
    float scanThickness; // �X�L�������̌��݁i�C�Ӂj
    int isRenderScanned; // �X�L�����ς݃��C����`�悷�邩�ǂ����̃t���O�i0�܂���1�j
};

ConstantBuffer<ScanEffectCB> gScanEffectCB : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{
    //PixelShaderOutput output;
    
    //output.color.rgb = input.color.rgb;
    
    //output.color.a = 1.0f;
    
    //return output;
    PixelShaderOutput output;

    float viewportHeight = 720.0f; // ��
    float yScreen = input.position.y / viewportHeight;

    float scanPos = gScanEffectCB.progress;
    float thickness = gScanEffectCB.scanThickness;

    // �X�L�������S����̋���
    float dist = abs(yScreen - scanPos);

    // ���S��1�i�j�A�[��0�i���j�ɂȂ�}�X�N
    float lineMask = saturate(1.0 - dist / thickness);

    // �X�L�����ς݃}�X�N�i0�܂���1�j
    float scannedMask = step(0.0f, scanPos - yScreen);

    int renderScanned = 0; // �X�L�����ς݃��C����`�悷�邩�ǂ����̃t���O�i0�܂���1�j
    
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
   
    
    // �F�ݒ�
    float3 scanColor = float3(0.0, 0.451, 0.871); // �X�L���������C���̐�
    float3 scannedColor = float3(1, 1, 1); // �X�L�����ςݔ�

    float3 color = scannedColor;

    //// �X�L�����ς݃��C���̓t���O�� true �̏ꍇ�̂ݕ`��
    //if (renderScanned)
    //{
    //    color = lerp(color, scannedColor, scannedMask);
    //}

    // �X�L���������C���𒆐S�E�[���̃O���f�[�V����
    color = lerp(color, scanColor, lineMask);

    output.color = float4(color, 1.0f);
    return output;
}