#include "KernelFile.h"

#include <iostream>

using namespace std;

KernelFile::KernelFile(std::string fileName, KernelPartition * kernelPartition, ClusterNo firstLevelIndexClusterNumber, BytesCnt fileSize, bool canWrite) {
	this->fileName = fileName;
	this->kernelPartition = kernelPartition;
	this->firstLevelIndexClusterNumber = firstLevelIndexClusterNumber;
	this->currentClusterNumber = firstLevelIndexClusterNumber;
	this->fileSize = fileSize;
	this->canWrite = canWrite;
	this->clusters.clear();
	this->secondLevelClusters.clear();
}

KernelFile::~KernelFile() {
	// TODO (acko): What to do here?
}

// ((2048 / 2) / 4) * 2048
const BytesCnt FIRST_LEVEL_INDEX_SIZE = 524288UL;

char KernelFile::write(BytesCnt count, char *buffer) {
	if (canWrite == false) {
		cout << "File not open for writting!" << endl;
		return '0';
	}
	if (firstLevelIndexClusterNumber == 0) {
		cout << "KernelFile::write() -> Need to allocate firstLevelIndex" << endl;
		auto bitVector = kernelPartition->fetchClusterFromPartition(0);
		auto firstLevelIndex = findFirstNotSet(bitVector, 2048);
		setBitValue(bitVector, firstLevelIndex, true);
		kernelPartition->saveClusterToPartition(0, bitVector);
		cout << "KernelFile::write() -> Allocated " << firstLevelIndex << endl;
		this->firstLevelIndexClusterNumber = firstLevelIndex;
		kernelPartition->updateIndexCluster(fileName, firstLevelIndex);
	}
	loadClustersFromPartition();
	for (int i = 0; i < count; i++) {
		auto currentClusterPosition = position % 2048;
		if (fileSize % 2048 == 0) {
			//cout << "KernelFile::write() -> Need to allocate new cluster" << endl;
			auto bitVector = kernelPartition->fetchClusterFromPartition(0);
			auto clusterNumber = findFirstNotSet(bitVector, 2048);
			setBitValue(bitVector, clusterNumber, true);
			kernelPartition->saveClusterToPartition(0, bitVector);
			//cout << "KernelFile::write() -> Allocated " << clusterNumber << endl;
			clusters.push_back(clusterNumber);
		}
		auto currentClusterNumber = clusters[position / 2048];
		auto currentClusterPartition = kernelPartition->fetchClusterFromPartition(currentClusterNumber);
		auto currentCluster = KernelCluster(currentClusterPartition);
		currentCluster.seek(currentClusterPosition);
		auto byte = buffer[i];
		currentCluster.writeByte(byte);
		kernelPartition->saveClusterToPartition(currentClusterNumber, currentClusterPartition);
		delete currentClusterPartition;
		position++;
		if (position > fileSize) {
			fileSize = position;
		}
	}
	saveClustersToPartition();
	kernelPartition->updateFileSize(fileName, fileSize);
	return '1';
}

BytesCnt KernelFile::read(BytesCnt count, char * buffer) {
	loadClustersFromPartition();
	cout << "KernelFile::read() fileName = " << fileName << " fileSize = " << fileSize << endl;
	cout << "numberOfClusters = " << clusters.size() << endl;
	for (int i = 0; i < count; i++) {
		if (position == fileSize) {
			return fileSize;
		}
		auto currentClusterPosition = position % 2048;
		auto currentClusterNumber = clusters[position / 2048];
		auto currentClusterPartition = kernelPartition->fetchClusterFromPartition(currentClusterNumber);
		auto currentCluster = KernelCluster(currentClusterPartition);
		currentCluster.seek(currentClusterPosition);
		auto byte = currentCluster.readByte();
		delete currentClusterPartition;
		position++;
		buffer[i] = byte;
	}
	return count;
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
	if (position == fileSize) {
		return '1';
	}
	else {
		return '0';
	}
}

BytesCnt KernelFile::getFileSize() {
	return fileSize;
}

char KernelFile::truncate() {
	loadClustersFromPartition();
	fileSize = position;
	auto currentClusterNumber = clusters[position / 2048];
	auto numberOfClusters = clusters.size();
	for (int i = currentClusterNumber + 1; i < numberOfClusters; i++) {
		clusters.pop_back();
	}
	saveClustersToPartition();
	return 0;
}

void KernelFile::loadClustersFromPartition() {
	clusters.clear();
	secondLevelClusters.clear();
	auto firstLevelIndexClusterPartition = kernelPartition->fetchClusterFromPartition(firstLevelIndexClusterNumber);
	auto firstLevelIndexCluster = KernelCluster(firstLevelIndexClusterPartition);
	while (firstLevelIndexCluster.getPosition() != 2044) {
		if (firstLevelIndexCluster.peekNumber() != 0) {
			auto clusterNumber = firstLevelIndexCluster.readNumber();
			clusters.push_back(clusterNumber);
		} else {
			break;
		}
	}
	// some second level shit is missing -_-
}

void KernelFile::saveClustersToPartition() {
	auto firstLevelIndexClusterPartition = kernelPartition->fetchClusterFromPartition(firstLevelIndexClusterNumber);
	auto firstLevelIndexCluster = KernelCluster(firstLevelIndexClusterPartition);
	auto numberOfClusters = clusters.size();
	for (int i = 0; i < numberOfClusters; i++) {
		firstLevelIndexCluster.writeNumber(clusters[i]);
		// check if larger then go into second clusters space WHOAAAAAAAAAAAAAAAAAAA
		// KALIFORNIKACIJA
	}
	kernelPartition->saveClusterToPartition(firstLevelIndexClusterNumber, firstLevelIndexClusterPartition);
	// first level
	// second level
	// second level cache... map<ClusterNo, vector<ClusterNo>>
}
