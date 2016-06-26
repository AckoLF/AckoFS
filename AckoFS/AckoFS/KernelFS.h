#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

#include "file.h"
#include "KernelPartition.h"

class KernelFS
{
public:
	KernelFS();
	~KernelFS();
	void lock();
	void unlock();
	char mount(Partition *partition);
	char unmount(char partitionSymbol);
	char format(char partitionSymbol);
	File* open(char *fileName, char mode);
	char readRootDir(char partitionSymbol, EntryNum entryNumber, Directory &directory);
	char doesExist(char *fileName);
	char deleteFile(char *fileName);
private:
	KernelPartition* getPartition(char partitionSymbol);
	HANDLE mutex;
	std::vector<KernelPartition*> mountedPartitions;
	friend class KernelFile;
};

