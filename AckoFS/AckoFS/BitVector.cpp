#include "BitVector.h"

// Notice me John Carmack senpai 
// https://en.wikipedia.org/wiki/Fast_inverse_square_root

void setBitValue(char *bitVector, ClusterNo clusterNumber, bool value) {
	bitVector[bitVectorIdx(clusterNumber)] ^= ((-value) ^ bitVector[bitVectorIdx(clusterNumber)]) & (1U << bitVectorPos(clusterNumber));
}

bool getBitValue(char *bitVector, ClusterNo clusterNumber) {
	return bitVector[bitVectorIdx(clusterNumber)] & (1U << bitVectorPos(clusterNumber));
}

int findFirstNotSet(char *bitVector, ClusterNo numberOfClusters) {
	auto bitVectorIndex = 0U;
	numberOfClusters >>= 3U;
	while (bitVectorIndex != numberOfClusters) {
		auto bitVectorValue = static_cast<unsigned char>((~(static_cast<unsigned char>(bitVector[bitVectorIndex]))));
		if (bitVectorValue != 0xFF) {
			auto bitVectorPosition = 8U;
			while (bitVectorValue != 0U) {
				bitVectorValue >>= 1U; 
				bitVectorPosition--;
			}
			return getClusterNumber(bitVectorIndex, bitVectorPosition);
		}
		else {
			bitVectorIndex++;
		}
	}
	// no free clusters
	return -1;
}
