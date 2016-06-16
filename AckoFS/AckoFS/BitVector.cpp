#include "BitVector.h"

// Notice me John Carmack senpai 
// https://en.wikipedia.org/wiki/Fast_inverse_square_root

void setBitValue(char * bitVector, ClusterNo clusterNumber, bool value) {
	bitVector[bitVectorIdx(clusterNumber)] ^= ((-value) ^ bitVector[bitVectorIdx(clusterNumber)]) & (1 << bitVectorPosition(clusterNumber));
}

bool getBitValue(char * bitVector, ClusterNo clusterNumber) {
	return bitVector[bitVectorIdx(clusterNumber)] & (1 << bitVectorPosition(clusterNumber));
}
