
Texture2D tex[3] : register(t0);
SamplerState splr[3] : register(s0);

cbuffer Time : register(b1) {
	float4 time;
};

struct VSIn {
	float2 texCoords : TEXCOORD;
	float2 screenPos : SCREENPOS;
	float4 position : SV_POSITION;
};

float4 main(VSIn vsi) : SV_TARGET {
	float2 distortion1 = tex[1].Sample(splr[1], float2(vsi.screenPos.x + time.x, vsi.screenPos.y)).xy * 2.0f - 1.0f;
	float2 distortion2 = tex[1].Sample(splr[1], float2(-vsi.screenPos.x, vsi.screenPos.y + time.x)).xy * 2.0f - 1.0f;

	float2 finalDistortion = (distortion1 + distortion2) * 0.01f;

	float waterRipple = tex[2].Sample(splr[2], vsi.screenPos).x * 0.3f;

	float4 outCol = tex[0].Sample(splr[0], vsi.texCoords + finalDistortion + float2(waterRipple, waterRipple));

	return outCol + float4(waterRipple, waterRipple, waterRipple, 0.0f);
} 
 