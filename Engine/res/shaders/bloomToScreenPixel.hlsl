
struct VSIn {
  float2 vUv      : VUV;
  float2 vL       : VL;
  float2 vR       : VR;
  float2 vT       : VT;
  float2 vB       : VB;
  float4 position : SV_POSITION;
};

Texture2D bloomTex : register(t0);
SamplerState bloomSplr : register(s0);

Texture2D source : register(t1);
SamplerState sourceSplr : register(s1);

Texture2D dither : register(t2);
SamplerState ditherSplr : register(s2);

cbuffer PData : register(b3) {
  float4 ditherTexel; // xy -> ditherScale, zw -> texelSize
}

float4 main(VSIn vsi) : SV_TARGET {
  float3 col = source.Sample(sourceSplr, vsi.vUv).rgb;
  
  {
    float3 lc = source.Sample(sourceSplr, vsi.vL).rgb;
    float3 rc = source.Sample(sourceSplr, vsi.vR).rgb;
    float3 tc = source.Sample(sourceSplr, vsi.vT).rgb;
    float3 bc = source.Sample(sourceSplr, vsi.vB).rgb;

    float dx = length(rc) - length(lc);
    float dy = length(tc) - length(bc);

    float3 n = normalize(float3(dx, dy, length(ditherTexel.zw)));
    float3 l = float3(0.0f, 0.0f, 1.0f);

    float diffuse = clamp(dot(n, l) + 0.7f, 0.7f, 1.0f);
    col *= diffuse;
  }
  
  float3 bloom = bloomTex.Sample(bloomSplr, vsi.vUv).rgb;

  float noise = dither.Sample(ditherSplr, vsi.vUv * ditherTexel.xy).r;
  noise = noise * 2.0f - 1.0f;
  bloom += noise / 255.0f;
  col += bloom;

  float a = max(col.r, max(col.g, col.b));
  return float4(col.r, col.g, col.b, a);
}
