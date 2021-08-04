
struct VSIn {
  float2 vUv      : VUV;
  float2 vL       : VL;
  float2 vR       : VR;
  float2 vT       : VT;
  float2 vB       : VB;
  float4 position : SV_POSITION;
};

Texture2D tex : register(t0);
SamplerState splr : register(s0);

cbuffer PData : register(b3) {
  float4 curveThreshold; // xyz = curve, w = threshold;
}

float4 main(VSIn vsi) : SV_TARGET {
  float3 curve = curveThreshold.rgb;
  float  threshold = curveThreshold.w;

  float3 c = tex.Sample(splr, vsi.vUv).rgb;
  float br = max(c.r, max(c.g, c.b));
  float rq = clamp(br - curve.x, 0.0f, curve.y);
  rq = curve.z * rq * rq;
  c *= max(rq, br - threshold) / max(br, 0.0001f);
  
  return float4(c.x, c.y, c.z, 0.0f);
}
