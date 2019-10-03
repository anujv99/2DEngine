
cbuffer Data : register(b1) {
	vector<uint, 2> TileData;
	vector<float, 2> Misc;
}

RWStructuredBuffer<float> b1 : register(u1);
RWStructuredBuffer<float> b2 : register(u2);

[numthreads(100, 10, 1)]
void main(uint3 threadID : SV_DISPATCHTHREADID) { 
	uint index = threadID.x + (threadID.y * TileData.x);

	b2[index] = ((
		b1[index + 1] +
		b1[index - 1] +
		b1[index + TileData.x] +
		b1[index - TileData.x] ) / 2.0f ) - b2[index];

	b2[index] *= Misc.x;
	
}
