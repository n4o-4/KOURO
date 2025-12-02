#include "Random.hlsli"
static uint kMaxParticles = 524288;

struct Particle
{
    float3 translate;
    float pad0; // 16バイト境界
    float3 scale;
    float pad1;
    float lifeTime;
    float3 pad2;
    float3 velocity;
    float pad3;
    float currentTime;
    float3 pad4;
    float4 color;
};

struct EmitterSphere
{
    float3 translate;    // 位置
    float radius;        // 射出半径
    uint count;          // 射出数
    float frequency;     // 射出間隔
    float frequencyTime; // 射出間隔調整用時間
    uint emit;           // 射出許可
};

struct PerView
{
    float4x4 viewProjection;
    float4x4 billboardMatrix;
};

struct PerFrame
{
    float time;
    
    float deltaTime;
};

struct VertexShaderOutput
{
    float4 position : SV_Position;
    float2 texcoord : TEXCOORD0;
    float4 color : COLOR0;
};

class RandomGenerator
{
    float3 seed;
    float3 Generate3d()
    {
        seed = rand3dTo3d(seed);
        return seed;
    }
    
    float Generate1d()
    {
        float result = rand3dTo1d(seed);
        seed.x = result;
        return result;
    }
};