#include "object3d.hlsli"

struct Material
{
    float4 color;             // 色
    int enableLighting;       // ライティングの有効化フラグ
    int enableEnvironmentMap; // 環境マップの有効化フラグ
    float4x4 uvTransform;     // UV変換行列
    float shininess;          // 鏡面反射の強さ
    float3 specularColor;     // 鏡面反射の色
};

struct DirectionLight
{
    float4 color;     // 色
    float3 direction; // 方向
    float intensity;  // 強度
};

struct PointLight
{
    float4 color;    // 色
    float3 position; // 位置
    float intensity; // 強度
    float radius;    // 影響範囲
    float decay;     // 減衰率
};

struct SpotLight
{
    float4 color;          // 色 
    float3 position;       // 位置
    float intensity;       // 強度
    float3 direction;      // 方向
    float distance;        // 影響範囲
    float decay;           // 減衰率
    float cosAngle;        // スポットライトの角度
    float cosFalloffStart; // スポットライトの角度の開始位置
};

ConstantBuffer<Material> gMaterial : register(b0);

ConstantBuffer<DirectionLight> gDirectionalLight : register(b1);

ConstantBuffer<PointLight> gPointLight : register(b2);

ConstantBuffer<SpotLight> gSpotLight : register(b3);

Texture2D<float4> gTexture : register(t0);

TextureCube<float4> gEnvironmentTexture : register(t1);

SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

// 平行光源計算
float3 CalculationDirectionalLight(VertexShaderOutput input)
{
    float3 resultColor;
    
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    float3 toEye = normalize(input.cameraPosition - input.worldPosition);
    
    float3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        
    float NDotH = dot(normalize(input.normal), halfVector);
        
    float specularPow = pow(saturate(NDotH), gMaterial.shininess);
    
    float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        
    float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);

    float3 directionalLight_Diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
    
    float3 directionalLight_Specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * gMaterial.specularColor;
    
    resultColor = directionalLight_Diffuse + directionalLight_Specular;
    
    return resultColor;
}

// 点光源計算
float3 CalculationPointLight(VertexShaderOutput input)
{
    float3 resultColor;
    
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    float3 toEye = normalize(input.cameraPosition - input.worldPosition);
  
    float distance = length(gPointLight.position - input.worldPosition);
        
    float factor = pow(saturate(-distance / gPointLight.radius + 1.0), gPointLight.decay);
        
    float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
        
    float3 pointLight_HalfVector = normalize(-pointLightDirection + toEye);
        
    float pointLight_NDotH = dot(normalize(input.normal), pointLight_HalfVector);
        
    float pointLight_SpecularPow = pow(saturate(pointLight_NDotH), gMaterial.shininess);
        
    float pointLight_NDotL = dot(normalize(input.normal), -pointLightDirection);
        
    float pointLight_Cos = pow(pointLight_NDotL * 0.5 + 0.5f, 2.0f);
       
    float3 pointLight_Diffuse = gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * pointLight_Cos * gPointLight.intensity * factor;
    
    float3 pointLight_Specular = gPointLight.color.rgb * gPointLight.intensity * pointLight_SpecularPow * factor * gMaterial.specularColor;
    
    resultColor = pointLight_Diffuse + pointLight_Specular;
    
    return resultColor;
}

// スポットライト計算
float3 CalculationSpottLight(VertexShaderOutput input)
{
    float3 resultColor;
    
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    float3 toEye = normalize(input.cameraPosition - input.worldPosition);
   
    float spotLight_Distance = length(gSpotLight.position - input.worldPosition); // �|�C���g���C�g�ւ̋���
        
    float attenuationFactor = pow(saturate(-spotLight_Distance / gSpotLight.distance + 1.0), gSpotLight.decay);
        
    float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);
        
    float cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
        
    float falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (gSpotLight.cosFalloffStart - gSpotLight.cosAngle));
        
    float3 spotLight_HalfVector = normalize(-gSpotLight.direction + toEye);
        
    float spotLight_NDotH = dot(normalize(input.normal), spotLight_HalfVector);
        
    float spotLight_SpecularPow = pow(saturate(spotLight_NDotH), gMaterial.shininess);
        
    float spotLight_NdotL = dot(normalize(input.normal), -gSpotLight.direction);
        
    float spotLight_Cos = pow(spotLight_NdotL * 0.5f + 0.5f, 2.0f);
    
    float3 spotLight_Diffuse = gMaterial.color.rgb * textureColor.rgb * gSpotLight.color.rgb * gSpotLight.intensity * falloffFactor * attenuationFactor * spotLight_Cos;
    
    float3 spotLight_Specular = gSpotLight.color.rgb * gSpotLight.intensity * spotLight_SpecularPow * falloffFactor * attenuationFactor * gMaterial.specularColor;
    
    resultColor = spotLight_Diffuse + spotLight_Specular;
    
    return resultColor;
}

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 transformedUV = mul(float4(input.texcoord,0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    // ライティングが有効なら計算を行う
    if (gMaterial.enableLighting != 0)
    {
        // カメラ位置からのベクトルを計算
        float3 toEye = normalize(input.cameraPosition - input.worldPosition);
        
        // 平行光源の計算
        float3 directionalLightColor = CalculationDirectionalLight(input);
        
        // 点光源の計算
        float3 pointLightColor = CalculationPointLight(input);
        
        // スポットライトの計算
        float3 spotLightColor = CalculationSpottLight(input);
       
        // 最終的な色の合成
        output.color.rgb = directionalLightColor + pointLightColor + spotLightColor;
        
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else // ライティングが無効なら、テクスチャの色をそのまま使用
    {
        
        output.color = gMaterial.color * textureColor;
    }
    
    // 環境マップが有効なら、反射ベクトルを計算して環境マップから色を取得
    if (gMaterial.enableEnvironmentMap != 0)
    {
        float3 cameraPosition = normalize(input.worldPosition - input.cameraPosition);
        float3 reflectionVector = reflect(cameraPosition, normalize(input.normal));
        float4 environmentColor = gEnvironmentTexture.Sample(gSampler, reflectionVector);
        
        output.color.rgb += environmentColor.rgb;
    }
    
    // 最終的な色を出力
    return output;
}

