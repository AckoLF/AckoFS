#pragma once

#include <unordered_map>
#include <string>

#include "part.h"
#include "file.h"
#include "BitVector.h"
#include "KernelCluster.h"
#include "KernelFile.h"

typedef std::string FileName;

class KernelPartition
{
public:
	KernelPartition(Partition *partition);
	~KernelPartition();
	File* open(std::string fileName, char mode);
	bool doesExist(std::string fileName);
	char deleteFile(std::string fileName);
	char readRootDir(EntryNum entryNumber, Directory & directory);
	char format();
public:
	Partition *partition;
	ClusterNo numberOfClusters;
	char* fetchClusterFromPartition(ClusterNo clusterNumber);
	void saveClusterToPartition(ClusterNo clusterNumber, char *cluster);
	bool isRootDirectoryIndexCorrupted();
	std::unordered_map<FileName, Entry*> fileEntries;
	char* createRootDirectoryIndex();
	void readRootDirectoryIndex();
	void writeRootDirectoryIndex();
	void updateIndexCluster(std::string fileName, ClusterNo indexCluster);
	void updateFileSize(std::string fileName, BytesCnt size);
	File* createFile(std::string fileName);
};

