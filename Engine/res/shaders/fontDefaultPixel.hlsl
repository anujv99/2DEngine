
Texture2D tex;
SamplerState splr;

struct VSIn {
	float2 texCoords : TEXCOORD;
	float4 position : SV_POSITION;
};

float4 main(VSIn vsi) : SV_TARGET {
	float4 outCol = tex.Sample(splr, vsi.texCoords);
	return outCol;
}
