#include "KernelPartition.h"

#include <iostream>

using namespace std;

KernelPartition::KernelPartition(Partition *partition) {
	this->partition = partition;
	this->numberOfClusters = partition->getNumOfClusters();

	if (isRootDirectoryIndexCorrupted()) {
		cout << "Partition is corrupted! Need to format it." << endl;
		this->format();
	} else {
		cout << "Partition is not corrupted!" << endl;
	}
}

KernelPartition::~KernelPartition() {
	// TODO(acko): Wait for all files to be closed.
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

	auto bitVector = new char[2048];
	partition->readCluster(0, bitVector);
	auto fileCluster = findFirstNotSet(bitVector, 2048);
	cout << "fileCluster is " << fileCluster << endl;
	// take it
	setBitValue(bitVector, fileCluster, true);
	partition->writeCluster(0, bitVector);

	fileEntry->splitRelativePath(fileName);
	fileEntry->reserved = 0;
	fileEntry->indexCluster = fileCluster;
	fileEntry->size = 0;
	fileEntries.insert({ fileName, fileEntry });

	// create file and return it

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
	// TODO(acko): Wait for all files to be closed.
	auto bitVector = new char[2048];
	memset(bitVector, 0, 2048);
	setBitValue(bitVector, 0, true);
	setBitValue(bitVector, 1, true);
	partition->writeCluster(0, bitVector);
	auto rootDirectoryIndex = createRootDirectoryIndex();
	partition->writeCluster(1, rootDirectoryIndex);
	return 0;
}

char* KernelPartition::fetchClusterFromPartition(ClusterNo clusterNumber) {
	auto cluster = new char[2048];
	partition->readCluster(clusterNumber, cluster);
	return cluster;
}

void KernelPartition::saveClusterToPartition(ClusterNo clusterNumber, char *cluster) {
	partition->writeCluster(clusterNumber, cluster);
}

bool KernelPartition::isRootDirectoryIndexCorrupted() {
	// bytes [1020, 1024) inside rootDirectoryIndex should be equal to acko
	auto partitionCluster = fetchClusterFromPartition(1);
	auto cluster = KernelCluster(partitionCluster);
	cluster.seek(1020);
	auto acko = cluster.readNumber();
	return acko != 1869308769UL;
}

char* KernelPartition::createRootDirectoryIndex() {
	auto partitionCluster = new char[2048];
	memset(partitionCluster, 0, 2048);
	auto cluster = KernelCluster(partitionCluster);
	cluster.seek(1020);
	cluster.writeByte('a');
	cluster.writeByte('c');
	cluster.writeByte('k');
	cluster.writeByte('o');
	return partitionCluster;
}

void KernelPartition::readRootDirectoryIndex() {
	// this should be stored inside cluster 1
	// fill it with random bullshit and test if this works 
	// then fill it with many many files and test if this works ;)

	auto partitionCluster = fetchClusterFromPartition(1);

	KernelCluster cluster(partitionCluster);
	// entries should start from 0, 20, 40, ..., 1000
	// then 1020-1024 is empty
	// then pointers... 

	// beware if there is random data
	while (cluster.getPosition() != 1020) {
		if (cluster.peekByte() != 0) {
			// there is something here
			auto clusterEntry = cluster.readClusterEntry();
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
			auto cachedSecondLevelCluster = fetchClusterFromPartition(secondLevelClusterIndex);
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
