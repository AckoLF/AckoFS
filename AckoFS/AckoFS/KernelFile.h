#pragma once

#include "fs.h"
#include "KernelPartition.h"

#include <string>
#include <vector>

class KernelFile
{
public:
	KernelFile(std::string fileName, KernelPartition *kernelPartition, ClusterNo firstLevelIndexClusterNumber, BytesCnt fileSize, bool canWrite);
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
	BytesCnt position;
	BytesCnt fileSize;
	ClusterNo firstLevelIndexClusterNumber, currentClusterNumber;
	bool canWrite;
	std::vector<ClusterNo> clusters, secondLevelClusters;
	void loadClustersFromPartition();
	void saveClustersToPartition();
};

