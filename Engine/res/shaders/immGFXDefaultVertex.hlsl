
cbuffer MVP {
	matrix <float, 4, 4> projection;
};

struct VSOut {
	float2 texCoords : TEXCOORD;
	float4 color : OUT_COLOR;
	float4 position : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float4 col : COLOR, float2 uv : TEXCOORDS) {
	VSOut vso;
	vso.position = mul(projection, float4(pos.xyz, 1.0f));
	vso.color = col;
	vso.texCoords = uv;
	return vso;
}
