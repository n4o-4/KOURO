#include "Particle.CS.hlsli"

RWStructuredBuffer<Particle> gParticles : register(u0);
RWStructuredBuffer<uint> gBaseUpdateList : register(u1);

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    //uint id = DTid.x;
    
    //Particle p = gParticles[id];
    
    //if (gBaseUpdateList[id] = (p.currentTime > 0.0f) || gBaseUpdateList[id] = (p.color.a > 0.0f))
    //{
        
    //}
}