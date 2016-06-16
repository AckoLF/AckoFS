#pragma once

#include "part.h"

#define bitVectorIdx(clusterNumber) ((clusterNumber) / 8)
#define bitVectorPosition(clusterNumber) (7 - ((clusterNumber) & 7))

void setBitValue(char *bitVector, ClusterNo clusterNumber, bool value);
bool getBitValue(char *bitVector, ClusterNo clusterNumber);


