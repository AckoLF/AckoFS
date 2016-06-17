#pragma once

#include <unordered_map>
#include <string>

#include "part.h"
#include "file.h"
#include "BitVector.h"

class KernelPartition
{
public:
	KernelPartition(Partition *partition);
	~KernelPartition();
	File* open(std::string fileName, char mode);
	char doesExist(std::string fileName);
	char deleteFile(std::string fileName);
	char readRootDir(EntryNum entryNumber, Directory & directory);
	char format();
private:
	Partition *partition;
	std::unordered_map<ClusterNo, char*> clusterCache;
	char* fetchClusterFromCache(ClusterNo clusterNumber);
	void dropClusterFromCache(ClusterNo clusterNumber);
	void saveClusterToPartition(ClusterNo clusterNumber);
};

