
struct VSOut {
	float4 color : OUT_COLOR;
	float4 position : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float4 col : COLOR) {
	VSOut vso;
	vso.position = float4(pos.xyz, 1.0f);
	vso.color = col;
	return vso;
}
