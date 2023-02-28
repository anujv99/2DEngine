RWStructuredBuffer<float> output: register(u1);
RWStructuredBuffer<float> diffused: register(u3);

cbuffer data : register(b1) {
  float4 res; // .xy = width, height; .z = evaporate value
}

[numthreads(32, 32, 1)]
void main( uint3 id: SV_DispatchThreadID ) {
  uint width = uint(res.x);
  uint height = uint(res.y);
  float evaporate = res.z;

  float finalCol = 0.0f;
  float total = 0.0f;

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      uint2 idx = uint2(id.x + i, id.y + j);
      if (idx.x < 0 || idx.x >= width || idx.y < 0 || idx.y >= height)
        continue;

      finalCol += output[uint(idx.y) * width + uint(idx.x)];
      total += 1;
    }
  }

  finalCol = max(0, finalCol - evaporate);

  uint outIdx = int(id.y) * width + int(id.x);
  diffused[outIdx] = (finalCol / total);
}
