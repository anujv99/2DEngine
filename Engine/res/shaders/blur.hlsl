
cbuffer Misc : register(b1) {
	int horizontal;
	float3 props;
};

Texture2D tex : register(t0);
SamplerState splr : register(s0);

struct VSIn {
	float2 texCoords : TEXCOORD;
	float4 position : SV_POSITION;
};

static float weights[5] = { 0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f };

float4 main(VSIn vsi) : SV_TARGET {
	float3 texCol = tex.Sample(splr, vsi.texCoords).xyz * weights[0];
	
	float width, height;
	tex.GetDimensions(width, height);
	
	float2 texOffset = 1.0f / float2(width, height);
	
	if (horizontal) {
		for (int i = 1; i < 5; i++) {
			texCol += tex.Sample(splr, vsi.texCoords + float2(texOffset.x * i, 0.0f)).xyz * weights[i];
			texCol += tex.Sample(splr, vsi.texCoords - float2(texOffset.x * i, 0.0f)).xyz * weights[i];
		}
	} else {
		for (int i = 1; i < 5; i++) {
			texCol += tex.Sample(splr, vsi.texCoords + float2(0.0f, texOffset.x * i)).xyz * weights[i];
			texCol += tex.Sample(splr, vsi.texCoords - float2(0.0f, texOffset.x * i)).xyz * weights[i];
		}
	}
	
	return float4(texCol, 1.0f);
}
