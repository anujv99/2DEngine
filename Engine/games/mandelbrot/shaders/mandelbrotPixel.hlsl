
Texture2D tex[16] : register(t0);
SamplerState splr[16] : register(s0);

struct VSIn {
	float2 texCoords : TEXCOORD;
	float4 color : OUT_COLOR;
	int texID : OUT_TEXTUREID;
	float4 position : SV_POSITION;
};

float4 main(VSIn vsi) : SV_TARGET{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
