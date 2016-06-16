#pragma once

#include <string>

#include "part.h"
#include "file.h"

class KernelPartition
{
public:
	KernelPartition(Partition *partition);
	~KernelPartition();
	File* open(std::string fileName, char mode);
	char doesExist(std::string fileName);
	char deleteFile(std::string fileName);
	char readRootDir(EntryNum entryNumber, Directory & directory);
private:
	Partition *partition;
};

