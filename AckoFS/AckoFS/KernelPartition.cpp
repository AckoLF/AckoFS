#include "KernelPartition.h"

using namespace std;

KernelPartition::KernelPartition(Partition *partition) {
	this->partition = partition;
}

KernelPartition::~KernelPartition() {
	// wait for all files to be closed!
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
