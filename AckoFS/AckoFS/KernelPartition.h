#pragma once

#include <unordered_map>
#include <string>

#include "part.h"
#include "file.h"
#include "BitVector.h"
#include "KernelCluster.h"
#include "KernelFile.h"
#include <Windows.h>

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
	/////////////////////////////////////////
	///// EKV - SINHRO //////////////////////
	/////////////////////////////////////////
	std::unordered_map<FileName, int> readerCount;
	std::unordered_map<FileName, HANDLE> readerMutex;
	std::unordered_map<FileName, HANDLE> writerMutex;
	std::unordered_map<FileName, int> threadCount;
	void startReader(FileName);
	void startWriter(FileName);
	void closeReaderWriter(FileName fileName, bool canWrite);
};

