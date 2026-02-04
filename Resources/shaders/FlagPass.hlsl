// FlagPass.hlsl
[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint id = DTid.x;
    //Particle p = particles[id];
    //flags[id] = (p.life > 0.0f) ? 1 : 0;
}