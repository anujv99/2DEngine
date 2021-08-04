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
  float4 intensity; // x -> intensity
}

float4 main(VSIn vsi) : SV_TARGET {
  float4 sum = float4(0.0f, 0.0f, 0.0f, 0.0f);
  sum += tex.Sample(splr, vsi.vL);
  sum += tex.Sample(splr, vsi.vR);
  sum += tex.Sample(splr, vsi.vT);
  sum += tex.Sample(splr, vsi.vB);
  sum *= 0.25f;

  return sum * intensity.x;
}