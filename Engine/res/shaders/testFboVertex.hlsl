
cbuffer VData : register(b9) {
	vector<uint, 4> Size;
}

struct VSOut {
	float2 texCoords : TEXCOORD;
	float2 screenPos : SCREEN_POS;
	float4 position : SV_POSITION;
};

VSOut main(float2 pos : POSITION, float2 uv : TEXCOORDS) {
	VSOut vso;
	vso.position = float4(pos.xy, 0.0f, 1.0f);
	vso.texCoords = uv;
	vso.screenPos = float2(abs(uv.x) * (float)Size.x, abs(uv.y) * (float)Size.y);
	return vso;
}