
Texture2D curr : register(t1);
SamplerState currSplr : register(s1);

Texture2D prev : register(t2);
SamplerState prevSplr : register(s2);

struct VSIn {
	float2 texCoords : TEXCOORD;
	float4 position : SV_POSITION;
};

static const float2 stepUV = float2(1.0f / 1600.0f, 1.0f / 900.0f);

float main(VSIn vsi) : SV_TARGET {
	float outCol = 0.0f;
	
	float right = 	prev.Sample(prevSplr, vsi.texCoords + float2(stepUV.x, 0.0f)).x;
	float left = 	prev.Sample(prevSplr, vsi.texCoords - float2(stepUV.x, 0.0f)).x;
	float top = 	prev.Sample(prevSplr, vsi.texCoords + float2(0.0f, stepUV.y)).x * 0.5f;
	float bottom = 	prev.Sample(prevSplr, vsi.texCoords - float2(0.0f, stepUV.y)).x * 0.5f;
	
	outCol = (right + left + top + bottom) * 0.5f;
	float currPixel = curr.Sample(currSplr, vsi.texCoords).x;
	
	outCol = outCol - currPixel;
	
	outCol *= 0.9f;

	return outCol;
} 
