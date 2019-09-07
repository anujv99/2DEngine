
cbuffer Data : register(b10) {
	vector<uint, 4> Size;
}

struct VSIn {
	float2 texCoords : TEXCOORD;
	float4 position : SV_POSITION;
};

StructuredBuffer<float> b : register(t10);

float4 main(VSIn vsi) : SV_TARGET {
	float outCol = b[uint(vsi.texCoords.x) + uint(vsi.texCoords.y) * Size.x] + 1.0f;
	return float4(outCol, outCol, outCol, outCol);
}
