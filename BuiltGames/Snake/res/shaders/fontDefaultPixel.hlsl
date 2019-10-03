
Texture2D tex;
SamplerState splr;

struct VSIn {
	float2 texCoords : TEXCOORD;
	float4 position : SV_POSITION;
};

float4 main(VSIn vsi) : SV_TARGET {
	float4 outCol = tex.Sample(splr, vsi.texCoords);
	if (outCol.a < 0.4) discard;
	return float4(1.0, 1.0, 1.0, 1.0);
}
