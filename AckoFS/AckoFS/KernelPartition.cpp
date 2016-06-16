#include "KernelPartition.h"

using namespace std;

KernelPartition::KernelPartition(Partition *partition) {
	this->partition = partition;
}


KernelPartition::~KernelPartition() {
}

File * KernelPartition::open(string fileName, char mode) {
	return nullptr;
}

char KernelPartition::doesExist(std::string fileName) {
	return 0;
}

char KernelPartition::deleteFile(std::string fileName) {
	return 0;
}

char KernelPartition::readRootDir(EntryNum entryNumber, Directory & directory) {
	return 0;
}
