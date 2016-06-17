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
		// make_pair
		// deep copy?
		clusterCache.insert({ clusterNumber, cluster });
	}
	
}
