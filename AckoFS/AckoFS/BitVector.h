#pragma once

#include "part.h"

#include <limits>

// TODO(acko) : Should you invert the bits or should you not?

#define bitVectorIdx(clusterNumber) ((clusterNumber) / 8U)
#define bitVectorPos(clusterNumber) (7U - ((clusterNumber) & 7U))
#define getClusterNumber(bitVectorIndex, bitVectorPosition) (((bitVectorIndex) << 3U) + (bitVectorPosition))

void setBitValue(char *bitVector, ClusterNo clusterNumber, bool value);
bool getBitValue(char *bitVector, ClusterNo clusterNumber);
unsigned int findFirstNotSet(char *bitVector, ClusterNo numberOfClusters);

