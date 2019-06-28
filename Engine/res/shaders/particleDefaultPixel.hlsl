
struct VSIn {
	float2 texCoords : TEXCOORD;
	float4 color : OUT_COLOR;
	float4 position : SV_POSITION;
};

float4 main(VSIn vsi) : SV_TARGET{
	float2 uvNorm = vsi.texCoords * 2.0f - float2(1.0f, 1.0f);
	float len = length(uvNorm);

	float alpha = 1.0f - smoothstep(0.0f, 1.0f, len);

	float4 finalColor = vsi.color;
	finalColor.w *= alpha;
	return finalColor;
}
