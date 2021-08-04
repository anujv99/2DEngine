
struct VSOut {
  float2 vUv      : VUV;
  float2 vL       : VL;
  float2 vR       : VR;
  float2 vT       : VT;
  float2 vB       : VB;
  float4 position : SV_POSITION;
};

cbuffer VData : register(b2) {
  float4 texelSize;
}

VSOut main( float2 pos : POSITION, float2 uv : TEXCOORDS /* unused */ ) {
  VSOut vso;
  vso.vUv = pos * 0.5f + 0.5f;
  vso.vUv.y = 1.0f - vso.vUv.y;
  vso.vL  = vso.vUv - float2(texelSize.x, 0.0f);
  vso.vR  = vso.vUv + float2(texelSize.x, 0.0f);
  vso.vT  = vso.vUv + float2(0.0f, texelSize.y);
  vso.vB  = vso.vUv - float2(0.0f, texelSize.y);
  vso.position = float4(pos.x, pos.y, 0.0f, 1.0f);
  return vso;
}
