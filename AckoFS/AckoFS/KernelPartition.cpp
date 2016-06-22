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
	// TODO(acko): Add synchronization and different file modes.
	// http://stackoverflow.com/questions/13064474/what-could-cause-a-deadlock-of-a-single-write-multiple-read-lock
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms686360(v=vs.85).aspx
	// https://msdn.microsoft.com/en-us/library/dd759350.aspx
	Entry *fileEntry = new Entry();
	auto bitVector = new char[2048];
	partition->readCluster(0, bitVector);
	auto firstLevelIndex = findFirstNotSet(bitVector, 2048);
	setBitValue(bitVector, firstLevelIndex, true);
	partition->writeCluster(0, bitVector);
	fileEntry->splitRelativePath(fileName);
	fileEntry->reserved = 0;
	fileEntry->indexCluster = firstLevelIndex;
	fileEntry->size = 0;
	fileEntries.insert({ fileName, fileEntry });
	// TODO(acko): Create a File and return it
	return nullptr;
}

char KernelPartition::doesExist(std::string fileName) {
	auto iterator = fileEntries.find(fileName);
	if (iterator == fileEntries.end()) {
		return '0';
	}
	else {
		return '1';
	}
}

char KernelPartition::deleteFile(std::string fileName) {
	auto doesExist = this->doesExist(fileName);
	if (!doesExist) {
		cout << "File " << fileName << " does not exist!" << endl;
		return '0';
	}
	auto iterator = fileEntries.find(fileName);
	auto bitVector = fetchClusterFromPartition(0);
	auto firstLevelIndex = iterator->second->indexCluster;
	setBitValue(bitVector, firstLevelIndex, false);
	auto firstLevelClusterPartition = fetchClusterFromPartition(firstLevelIndex);
	auto firstLevelCluster = KernelCluster(firstLevelClusterPartition);
	// TODO(acko): Do you need to delete everything?
	firstLevelCluster.seek(1024);
	while (firstLevelCluster.getPosition() != 2048) {
		auto secondLevelIndex = firstLevelCluster.readNumber();
		if ((secondLevelIndex > 0) && (secondLevelIndex < numberOfClusters)) {
			setBitValue(bitVector, secondLevelIndex, false);
		} else {
			break;
		}
	}
	fileEntries.erase(iterator);
	saveClusterToPartition(0, bitVector);
	writeRootDirectoryIndex();
	return '1';
}

char KernelPartition::readRootDir(EntryNum entryNumber, Directory & directory) {
	readRootDirectoryIndex();
	char count = 0;
	// TODO (acko): Double add everything to vector, fuck it.
	for (auto fileEntry : fileEntries) {
		if (count >= entryNumber) {
			if (count == 64) {
				return 65;
			}
			else {
				directory[count - entryNumber] = *(fileEntry.second);
			}
		}
		count++;
	}
	return count;
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
	auto partitionCluster = fetchClusterFromPartition(1);
	auto cluster = KernelCluster(partitionCluster);
	cluster.seek(1020);
	auto acko = cluster.readNumber();
	return acko != 1869308769UL;
}

char* KernelPartition::createRootDirectoryIndex() {
	fileEntries.clear();
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
	fileEntries.clear();
	auto partitionCluster = fetchClusterFromPartition(1);
	KernelCluster cluster(partitionCluster);
	while (cluster.getPosition() != 1020) {
		if (cluster.peekByte() != 0) {
			auto clusterEntry = new Entry(cluster.readClusterEntry());
			// TODO (acko): Assert that it is unique
			fileEntries[clusterEntry->getRelativePath()] = clusterEntry;
		}
		else {
			break;
		}
	}
	cluster.seek(1024);
	while (cluster.getPosition() != 2048) {
		if (cluster.peekNumber() != 0) {
			auto secondLevelClusterIndex = cluster.readNumber();
			auto cachedSecondLevelCluster = fetchClusterFromPartition(secondLevelClusterIndex);
			KernelCluster secondLevelCluster(cachedSecondLevelCluster);
			while (secondLevelCluster.getPosition() != 2040) {
				if (secondLevelCluster.peekByte() != 0) {
					auto clusterEntry = new Entry(secondLevelCluster.readClusterEntry());
					fileEntries[clusterEntry->getRelativePath()] = clusterEntry;
				}
				else {
					break;
				}
			}
		}
		else {	
			break;
		}
	}
}

void KernelPartition::writeRootDirectoryIndex() {
	auto rootDirectoryIndex = new char[2048];
	memset(rootDirectoryIndex, 0, 2048);
	auto rootDirectoryIndexCluster = KernelCluster(rootDirectoryIndex);
	for (auto fileEntry : fileEntries) {
		// TODO(acko): Oh shit... Add second level stuff here 
		rootDirectoryIndexCluster.writeClusterEntry(*(fileEntry.second));
	}
	rootDirectoryIndexCluster.seek(1020);
	rootDirectoryIndexCluster.writeByte('a');
	rootDirectoryIndexCluster.writeByte('c');
	rootDirectoryIndexCluster.writeByte('k');
	rootDirectoryIndexCluster.writeByte('o');
	saveClusterToPartition(1, rootDirectoryIndex);
}
