#include "Particle.CS.hlsli"

RWStructuredBuffer<Particle> gParticles : register(u0);
RWStructuredBuffer<int> gFreeCounter : register(u1);
ConstantBuffer<EmitterSphere> gEmitter : register(b0);
ConstantBuffer<PerFrame> gPerFrame : register(b1);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if(gEmitter.emit != 0)
    {
        RandomGenerator generator;
        generator.seed = dot(DTid, uint3(1, 738, 193)) + gPerFrame.time;
        
        for (uint countIndex = 0; countIndex < gEmitter.count;countIndex++)
        {     
            int particleIndex;
            InterlockedAdd(gFreeCounter[0], 1, particleIndex);
            
            if (particleIndex < kMaxParticles)
            {
                gParticles[particleIndex].scale = generator.Generate3d();
                gParticles[particleIndex].translate = generator.Generate3d() * 11 - 6;
                gParticles[particleIndex].velocity = generator.Generate3d() * 0.7 - 0.4;
                gParticles[particleIndex].color.rgb = generator.Generate3d();
                gParticles[particleIndex].lifeTime = 10;
                gParticles[particleIndex].color.a = 1.0f;
            }
        }
    }
}