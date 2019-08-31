
cbuffer Size : register(b1) {
	vector<uint, 2> tileSize;
	vector<float, 2> damping;
}

RWStructuredBuffer<float> buffer1 : register(u0);
RWStructuredBuffer<float> buffer2 : register(u1);

[numthreads(1, 1, 1)]
void main(uint3 threadIDInGroup : SV_GroupThreadID, uint3 groupID : SV_GroupID) {
	if (groupID.x > 0 && groupID.x < tileSize.x && groupID.y > 0 && groupID.y < tileSize.y) {
		buffer2[groupID.x + groupID.y * tileSize.y] = 
			( buffer1[(groupID.x - 1) + ((groupID.y    ) * tileSize.y)] +
			  buffer1[(groupID.x + 1) + ((groupID.y    ) * tileSize.y)] +
			  buffer1[(groupID.x    ) + ((groupID.y + 1) * tileSize.y)] +
			  buffer1[(groupID.x    ) + ((groupID.y - 1) * tileSize.y)] ) / 2 - buffer2[groupID.x + groupID.y * tileSize.y];

		buffer2[groupID.x + groupID.y * tileSize.y] *= damping.x;
	}
}