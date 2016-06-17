#pragma once

#include <cstring>

#include "fs.h"

class KernelCluster {
private:
	char *cluster;
	unsigned int position;
public:
	KernelCluster(char *cluster);
	~KernelCluster() = default;
	unsigned int getPosition();
	void seek(unsigned int position);
	char readByte();
	void writeByte(char byte);
	unsigned long readNumber();
	void writeNumber(unsigned long number);
	Entry readClusterEntry();
	void writeClusterEntry(Entry clusterEntry);
};

