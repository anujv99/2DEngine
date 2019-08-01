
Texture2D tex : register(t0);
SamplerState splr : register(s0);

struct VSIn {
	float2 texCoords : TEXCOORD;
	float4 position : SV_POSITION;
};

float4 main(VSIn vsi) : SV_TARGET {
	float4 outCol = tex.Sample(splr, vsi.texCoords);
	float brightness = dot(outCol.rgb, float3(1.0f, 1.0f, 1.0f));
	if (brightness > 1.0)
		return outCol;
	else
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
}
