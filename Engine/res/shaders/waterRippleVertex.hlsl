
struct VSOut {
	float2 texCoords : TEXCOORD;
	float2 screenPos : SCREENPOS;
	float4 position : SV_POSITION;
};

VSOut main(float2 pos : POSITION, float2 uv : TEXCOORDS) {
	VSOut vso;
	vso.position = float4(pos.xy, 0.0f, 1.0f);
	vso.texCoords = (uv * float2(1.0f, -1.0f)) + float2(0.0f, 1.0f);
	vso.screenPos = (pos.xy + float2(1.0f, -1.0f)) / float2(2.0f, -2.0f);
	return vso;
}
