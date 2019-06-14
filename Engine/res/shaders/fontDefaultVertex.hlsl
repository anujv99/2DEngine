
cbuffer MVP {
	matrix <float, 4, 4> projection;
};

struct VSOut {
	float2 texCoords : TEXCOORD;
	float4 position : SV_POSITION;
};

VSOut main(float2 pos : FONT_POSITION, float2 uv : FONT_TEXTURECOORD) {
	VSOut vso;
	vso.position = mul(projection, float4(pos.x , pos.y , 0.0, 1.0));
	vso.texCoords = uv;
	return vso;
}
