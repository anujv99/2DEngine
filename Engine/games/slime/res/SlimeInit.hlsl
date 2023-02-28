#define PI 3.1415926538

cbuffer data : register(b1) {
  float4 res;
}

struct Agent {
  float2 pos;
  float2 angle; // .x = angle
};

RWStructuredBuffer<Agent> agents: register(u2);

[numthreads(1, 1, 1)]
void main( uint3 id: SV_DispatchThreadID ) {
  uint width = uint(res.x);
  uint height = uint(res.y);
  uint idx = id.x;

  agents[idx].pos = float2(float(width) / 2.0f, float(height) / 2.0f);
  agents[idx].angle.x = PI * id.x;
}
