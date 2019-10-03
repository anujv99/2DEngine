
struct VSOut {
	float2 texCoords : TEXCOORD;
	float4 position : SV_POSITION;
};

VSOut main(float2 pos : POSITION, float2 uv : TEXCOORDS) {
	VSOut vso;
	vso.position = float4(pos.xy, 0.0f, 1.0f);
	vso.texCoords = uv;
	return vso;
}