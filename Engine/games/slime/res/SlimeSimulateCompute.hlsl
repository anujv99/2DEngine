
#define PI 3.1415926538

cbuffer data : register(b1) {
  float4 res; // .xy = width, height; .z = angle spacing, .w = turn speed
  float4 sensorData;
}

struct Agent {
  float2 pos;
  float2 angle; // .x = angle
};

RWStructuredBuffer<float> output: register(u1);
RWStructuredBuffer<Agent> agents: register(u2);

// Hash function from H. Schechter & R. Bridson, goo.gl/RXiKaH
uint hash(uint s) {
  s ^= 2747636419u;
  s *= 2654435769u;
  s ^= s >> 16;
  s *= 2654435769u;
  s ^= s >> 16;
  s *= 2654435769u;
  return s;
}

float rand(uint seed) {
  return float(hash(seed)) / 4294967295.0f;
}

float sense(Agent a, float angleOffset) {
  uint width = res.x;
  uint height = res.y;

  float sensorOffsetDist = sensorData.x;
  int sensorSize = sensorData.y;

  float sensorAngle = a.angle.x + angleOffset;
  float2 sensorDir = float2(cos(sensorAngle), sin(sensorAngle));
  float2 sensorCenter = a.pos + sensorDir * sensorOffsetDist;
  float sum = 0;

  for (int i = -sensorSize; i <= sensorSize; i++) {
    for (int j = -sensorSize; j <= sensorSize; j++) {
      int2 idx = int2(sensorCenter.x + i, sensorCenter.y + j);
      if (idx.x < 0 || idx.x >= width || idx.y < 0 || idx.y >= height)
        continue;

      sum += output[uint(idx.y) * width + uint(idx.x)];
    }
  }

  return sum;
}

[numthreads(1024, 1, 1)]
void main( uint3 id : SV_DispatchThreadID ) {
  uint width = res.x;
  uint height = res.y;
  float sensorAngleSpacing = res.z;
  float turnSpeed = res.w;

  uint idx = id.x;
  Agent agent = agents[idx];
  float2 pos = agent.pos;
  float angle = agent.angle.x;
  float random = rand(uint(pos.x * width + pos.y + hash(id.x)));

  uint outIdx = int(pos.y) * width + int(pos.x);
  output[outIdx] = 1.0f;

  // change angle
  float forward = sense(agent, 0);
  float left = sense(agent, sensorAngleSpacing);
  float right = sense(agent, -sensorAngleSpacing);
  
  if (forward > left && forward > right) {
    angle += 0;
  } else if (forward < left && forward < right) {
    angle += (random - 0.5f) * 2 * turnSpeed;
  } else if (right > left) {
    angle -= random * turnSpeed;
  } else if (left > right) {
    angle += random * turnSpeed;
  }

  agents[idx].angle.x = angle;

  // update agent position
  float2 delta = float2(cos(angle), sin(angle));
  float2 newPos = float2(pos.x + delta.x, pos.y + delta.y);

  if (newPos.x < 0 || newPos.x >= width || newPos.y < 0 || newPos.y >= height) {
    newPos.x = clamp(newPos.x, 0, width - 1);
    newPos.y = clamp(newPos.y, 0, height - 1);
    agents[idx].angle.x = random * 2 * PI;
  }

  agents[idx].pos = newPos;
}