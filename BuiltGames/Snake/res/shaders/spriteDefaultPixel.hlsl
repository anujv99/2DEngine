
Texture2D tex[16] : register(t0);
SamplerState splr[16] : register(s0);

struct VSIn {
	float2 texCoords : TEXCOORD;
	float4 color : OUT_COLOR;
	int texID : OUT_TEXTUREID;
	float4 position : SV_POSITION;
};

float4 main(VSIn vsi) : SV_TARGET {
	float4 outCol;
	if (vsi.texID < 0) {
		outCol = vsi.color;
	} else {
		switch (vsi.texID) {
		case 0:
			outCol = tex[0].Sample(splr[0], vsi.texCoords) * vsi.color;
			break;
		case 1:
			outCol = tex[1].Sample(splr[1], vsi.texCoords) * vsi.color;
			break;
		case 2:
			outCol = tex[2].Sample(splr[2], vsi.texCoords) * vsi.color;
			break;
		case 3:
			outCol = tex[3].Sample(splr[3], vsi.texCoords) * vsi.color;
			break;
		case 4:
			outCol = tex[4].Sample(splr[4], vsi.texCoords) * vsi.color;
			break;
		case 5:
			outCol = tex[5].Sample(splr[5], vsi.texCoords) * vsi.color;
			break;
		}
	}
	return outCol;
}
