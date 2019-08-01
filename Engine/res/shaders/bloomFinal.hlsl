
Texture2D tex : register(t0);
SamplerState splr : register(s0);

Texture2D blur : register(t1);
SamplerState blursplr : register(s1);

struct VSIn {
	float2 texCoords : TEXCOORD;
	float4 position : SV_POSITION;
};

float4 main(VSIn vsi) : SV_TARGET {
	float3 texColor = tex.Sample(splr, vsi.texCoords);
	float3 blrColor = blur.Sample(blursplr, vsi.texCoords);
	
	texColor += blrColor;
	
	return float4(texColor, 1.0f); 
	
}
