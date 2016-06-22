#pragma once

#include <unordered_map>
#include <string>

#include "part.h"
#include "file.h"
#include "BitVector.h"
#include "KernelCluster.h"

typedef std::string FileName;

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
	ClusterNo numberOfClusters;
	char* fetchClusterFromPartition(ClusterNo clusterNumber);
	void saveClusterToPartition(ClusterNo clusterNumber, char *cluster);
	bool isRootDirectoryIndexCorrupted();
	// need to add some files bro
	std::unordered_map<FileName, Entry*> fileEntries;
	// also handle index
	char* createRootDirectoryIndex();
	void readRootDirectoryIndex();
	void writeRootDirectoryIndex();
};

