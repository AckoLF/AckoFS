#pragma once

#include "fs.h"
#include "KernelPartition.h"

class KernelFile
{
public:
	KernelFile(std::string fileName, KernelPartition *kernelPartition, ClusterNo firstLevelIndexClusterNumber, BytesCnt size, bool canWrite);
	~KernelFile();
	char write(BytesCnt, char* buffer);
	BytesCnt read(BytesCnt, char* buffer);
	char seek(BytesCnt);
	BytesCnt filePos();
	char eof();
	BytesCnt getFileSize();
	char truncate();
private:
	std::string fileName;
	KernelPartition *kernelPartition;
	BytesCnt position, size;
	ClusterNo firstLevelIndexClusterNumber, curretClusterNumber;
	bool canWrite;
	ClusterNo numberOfSecondLevelClusters();
};

