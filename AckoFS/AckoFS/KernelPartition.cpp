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
	// maybe read root directory?
	auto iterator = fileEntries.find(fileName);
	if (iterator == fileEntries.end()) {
		return '0';
	}
	else {
		return '1';
	}
}

char KernelPartition::deleteFile(std::string fileName) {
	return 0;
}

char KernelPartition::readRootDir(EntryNum entryNumber, Directory & directory) {
	for (const auto& fileEntry : fileEntries) {
		cout << fileEntry.first << " " << fileEntry.second->toString() << endl;
	}
	createRootDirectoryIndex();
	return '1';
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

void KernelPartition::createRootDirectoryIndex() {
	// this should be stored inside cluster 1
	// fill it with random bullshit and test if this works 
	// then fill it with many many files and test if this works ;)
	auto cashedCluster = fetchClusterFromCache(1);


	for (int i = 0; i < 25; i++) {
		printf("%d ", static_cast<int>(static_cast<unsigned char>(cashedCluster[i])));
	}
	printf("\n");

	KernelCluster cluster(cashedCluster);
	// entries should start from 0, 20, 40, ..., 1000
	// then 1020-1024 is empty
	// then pointers... 

	// beware if there is random data
	while (cluster.getPosition() != 1020) {
		if (cluster.peekByte() != 0) {
			// there is something here
			auto clusterEntry = cluster.readClusterEntry();
			cout << clusterEntry.toString() << " i can read this" << endl;
			// add this to files
		}
		else {
			// nothing to read, EOF
			break;
		}
	}
	if (cluster.getPosition() == 1020) {
		cluster.readNumber();
		// skip 4 bytes of noise
	}
	cout << cluster.getPosition() << endl;
	while (cluster.getPosition() != 2048) {
		if (cluster.peekNumber() != 0) {
			// there is something here
			auto secondLevelClusterIndex = cluster.readNumber();
			auto cachedSecondLevelCluster = fetchClusterFromCache(secondLevelClusterIndex);
			KernelCluster secondLevelCluster(cachedSecondLevelCluster);
			cout << "index is: " << secondLevelClusterIndex << endl;
			while (secondLevelCluster.getPosition() != 2040) {
				if (secondLevelCluster.peekByte() != 0) {
					// there is a file
					auto clusterEntry = secondLevelCluster.readClusterEntry();
					cout << clusterEntry.toString() << " this is some second level shit here" << endl;
					// add this =)
					break;
				}
				else {
					break;
				}
			}
		}
		else {
			// nothing to read, EOF
	
			break;
		}
	}
	// read pointers then read clusters 
}
