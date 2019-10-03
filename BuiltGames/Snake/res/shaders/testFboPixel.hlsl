
Texture2D tex : register(t0);
SamplerState splr : register(s0);

cbuffer PData : register(b10) {
	vector<uint, 4> Size;
}

struct VSIn {
	float2 texCoords : TEXCOORD;
	float2 screenPos : SCREEN_POS;
	float4 position : SV_POSITION;
};

StructuredBuffer<float> b : register(t10);

float4 main(VSIn vsi) : SV_TARGET {
	float offset = b[uint(vsi.screenPos.x) + uint(vsi.screenPos.y) * Size.x] / 10.0f;

	float4 outCol = tex.Sample(splr, vsi.texCoords + float2(offset, offset));
	return outCol + float4(1.0f, 1.0f, 1.0f, 1.0f) * offset;
}
