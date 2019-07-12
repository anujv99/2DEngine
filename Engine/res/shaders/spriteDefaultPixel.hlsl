
Texture2D tex[32] : register(t0);
SamplerState splr[32] : register(s0);

struct VSIn {
	float2 texCoords : TEXCOORD;
	float4 color : OUT_COLOR;
	int texID : OUT_TEXTUREID;
	float4 position : SV_POSITION;
};

float4 main(VSIn vsi) : SV_TARGET{
	float4 outCol;
	if (vsi.texID < 0) {
		outCol = vsi.color;
	} else {
		switch (vsi.texID) {
		case 0:
			outCol = tex[0].Sample(splr[0], vsi.texCoords);
			break;
		}
	}
	return outCol;
}
