#pragma once

#include "fs.h"
#include "KernelPartition.h"
#include <Windows.h>

#include <string>
#include <vector>

class KernelFile
{
public:
	KernelFile(std::string fileName, KernelPartition *kernelPartition, ClusterNo firstLevelIndexClusterNumber, BytesCnt fileSize, bool canWrite, bool canRead);
	~KernelFile();
	char write(BytesCnt, char* buffer);
	BytesCnt read(BytesCnt, char* buffer);
	char seek(BytesCnt);
	BytesCnt filePos();
	char eof();
	BytesCnt getFileSize();
	char truncate();
	void lock();
	void unlock();
private:	
	HANDLE mutex;
	std::string fileName;
	KernelPartition *kernelPartition;
	BytesCnt position;
	BytesCnt fileSize;
	ClusterNo firstLevelIndexClusterNumber, currentClusterNumber;
	bool canWrite;
	bool canRead;
	std::vector<ClusterNo> clusters, secondLevelClusters;
	void loadClustersFromPartition();
	void saveClustersToPartition();
};

