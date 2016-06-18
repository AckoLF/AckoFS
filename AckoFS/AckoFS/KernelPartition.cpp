#include "KernelPartition.h"

#include <iostream>

using namespace std;

KernelPartition::KernelPartition(Partition *partition) {
	this->partition = partition;
	this->numberOfClusters = partition->getNumOfClusters();
	this->rootDirectoryIndexStart = this->numberOfClusters / 8;
}

KernelPartition::~KernelPartition() {
	// wait for all files to be closed!
}

File * KernelPartition::open(string fileName, char mode) {
	// for now handle only write mode 
	// use ReaderWriter locks please here ;)
	// http://stackoverflow.com/questions/13064474/what-could-cause-a-deadlock-of-a-single-write-multiple-read-lock
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms686360(v=vs.85).aspx
	// https://msdn.microsoft.com/en-us/library/dd759350.aspx

	// maybe read root index (?)
	// this code is actually createFile function :) 

	Entry *fileEntry = new Entry();

	// find a cluster for the file
	// separate into a function

	auto bitVector = fetchClusterFromCache(0);
	auto fileCluster = findFirstNotSet(bitVector, 2048);
	cout << "fileCluster is " << fileCluster << endl;
	// take it
	setBitValue(bitVector, fileCluster, true);

	fileEntry->splitRelativePath(fileName);
	fileEntry->reserved = 0;
	fileEntry->indexCluster = fileCluster;
	fileEntry->size = 0;
	fileEntries.insert({ fileName, fileEntry });
	return nullptr;
}

char KernelPartition::doesExist(std::string fileName) {
	return 0;
}

char KernelPartition::deleteFile(std::string fileName) {
	return 0;
}

char KernelPartition::readRootDir(EntryNum entryNumber, Directory & directory) {
	for (const auto& fileEntry : fileEntries) {
		cout << fileEntry.first << " " << fileEntry.second->toString() << endl;
	}
	return 0;
}

char KernelPartition::format() {
	// clear cache
	// set bit vector

	// wait for all files to be closed!

	auto bitVector = new char[2048];
	// hahaha, mlatka, sizeof ne radi
	memset(bitVector, 0, 2048);
	setBitValue(bitVector, 0, true);
	partition->writeCluster(0, bitVector);

	clusterCache.clear();
	return 0;
}

char * KernelPartition::fetchClusterFromCache(ClusterNo clusterNumber) {
	auto iterator = clusterCache.find(clusterNumber);
	if (iterator != clusterCache.end()) {
		return iterator->second;
	}
	else {
		auto cluster = new char[2048];
		partition->readCluster(clusterNumber, cluster);
		auto cachedCluster = new char[2048];
		memcpy(cachedCluster, cluster, 2048);
		clusterCache.insert({ clusterNumber, cachedCluster });
		return cachedCluster;
	}
}

void KernelPartition::dropClusterFromCache(ClusterNo clusterNumber) {
	auto iterator = clusterCache.find(clusterNumber);
	if (iterator != clusterCache.end()) {
		// wait, there is more, update bitvector!
		clusterCache.erase(iterator);
	}
}

void KernelPartition::saveClusterToPartition(ClusterNo clusterNumber) {
	auto iterator = clusterCache.find(clusterNumber);
	if (iterator != clusterCache.end()) {
		partition->writeCluster(clusterNumber, iterator->second);
	} else {
		// this is very bad, fatal error
	}
}
