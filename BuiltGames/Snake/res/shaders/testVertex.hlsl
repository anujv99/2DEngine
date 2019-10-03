
struct VSOut {
	float2 texCoords : TEXCOORD;
	float4 position : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float2 uv : TEXCOORDS, float4 col : COLOR, int texID : TEXTUREID) {
	VSOut vso;
	vso.position = float4(pos, 1.0f);
	vso.texCoords = float2(abs(uv.x), abs(uv.y));
	return vso;
}
