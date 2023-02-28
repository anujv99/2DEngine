
StructuredBuffer<float> output: register(t1);

cbuffer data : register(b1) {
  float4 res;
  float4 color;
}

struct VSIn {
	float2 texCoords : TEXCOORD;
	float4 color : OUT_COLOR;
	float4 position : SV_POSITION;
};

float4 main( VSIn vsi ): SV_TARGET{
  uint width = res.x;
  uint height = res.y;

  uint x = uint(vsi.texCoords.x * width);
  uint y = uint(vsi.texCoords.y * height);

  float outCol = output[y * width + x];

  return color * float4(outCol, outCol, outCol, outCol);
}
