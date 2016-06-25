#include "KernelFile.h"

#include <iostream>

using namespace std;

KernelFile::KernelFile(std::string fileName, KernelPartition * kernelPartition, ClusterNo firstLevelIndexClusterNumber, BytesCnt size, bool canWrite) {
	this->fileName = fileName;
	this->kernelPartition = kernelPartition;
	this->firstLevelIndexClusterNumber = firstLevelIndexClusterNumber;
	this->curretClusterNumber = firstLevelIndexClusterNumber;
	this->size = size;
	this->canWrite = canWrite;
}

KernelFile::~KernelFile() {
	// TODO (acko): What to do here?
}

char KernelFile::write(BytesCnt count, char * buffer) {
	if (canWrite == false) {
		cout << "File not open for writting!" << endl;
		return '0';
	}
	if (firstLevelIndexClusterNumber == 0) {
		cout << "Need to allocate firstLevelIndex" << endl;
		auto bitVector = kernelPartition->fetchClusterFromPartition(0);
		auto firstLevelIndex = findFirstNotSet(bitVector, 2048);
		setBitValue(bitVector, firstLevelIndex, true);
		kernelPartition->saveClusterToPartition(0, bitVector);
		cout << "allocated " << firstLevelIndex << endl;
		kernelPartition->updateIndexCluster(fileName, firstLevelIndex);
		kernelPartition->updateFileSize(fileName, 5);
	}
	return '1';
}

BytesCnt KernelFile::read(BytesCnt, char * buffer) {
	return BytesCnt();
}

char KernelFile::seek(BytesCnt position) {
	// TODO(acko): Check if position is legal 
	this->position = position;
	return '1';
}

BytesCnt KernelFile::filePos() {
	return position;
}

char KernelFile::eof() {
	if (position == size) {
		return '1';
	}
	else {
		return '0';
	}
}

BytesCnt KernelFile::getFileSize() {
	return size;
}

char KernelFile::truncate() {
	return 0;
}

ClusterNo KernelFile::numberOfSecondLevelClusters() {
	if (size <= 1024) {
		return 0;
	}
	else {
		return ((size - 1024) + 2047) / 2048;
	}
}
