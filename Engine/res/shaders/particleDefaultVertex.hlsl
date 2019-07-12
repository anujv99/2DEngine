
cbuffer MVP : register(b0) {
	matrix <float, 4, 4> projection;
};

struct VSOut {
	float2 texCoords : TEXCOORD;
	float4 color : OUT_COLOR;
	int texID : OUT_TEXTUREID;
	float4 position : SV_POSITION;
};

VSOut main(float2 pos : POSITION, float2 uv : TEXCOORDS, float4 col : COLOR) {
	VSOut vso;
	vso.position = mul(projection, float4(pos.xy, 0.0f, 1.0f));
	vso.color = col;
	vso.texCoords = (uv * float2(1, -1)) + float2(-0.5, 0.5);
	return vso;
}
