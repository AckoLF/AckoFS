#include "KernelPartition.h"

#include <iostream>

#include "KernelFS.h"

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
	
	auto result = new File();
	if (mode == 'r') {
		if (doesExist(fileName) == false) {
			cout << "File " << fileName << " doesn't exist and can't be open for read!" << endl;
			return nullptr;
		} else {
			auto fileEntry = fileEntries[fileName];
			result->myImpl = new KernelFile(fileName, this, fileEntry->indexCluster, fileEntry->size, false, true);
			return result;
		}
	}
	else if (mode == 'w') {
		if (doesExist(fileName) == true) {
			deleteFile(fileName);
		}
		return createFile(fileName);
	}
	else if (mode == 'a') {
		if (doesExist(fileName) == false) {
			cout << "File " << fileName << " doesn't exist and can't be open for append!" << endl;
			return nullptr;
		}
		else {
			auto fileEntry = fileEntries[fileName];
			result->myImpl = new KernelFile(fileName, this, fileEntry->indexCluster, fileEntry->size, true, true);
			result->myImpl->seek(fileEntry->size);
			return result;
		}
	}
	else {
		throw "up";
	}
}

File * KernelPartition::createFile(std::string fileName) {
	Entry	*fileEntry = new Entry();
	fileEntry->splitRelativePath(fileName);
	fileEntry->reserved = 0;
	fileEntry->indexCluster = 0;
	fileEntry->size = 0;
	fileEntries.insert({ fileName, fileEntry });
	writeRootDirectoryIndex();
	auto result = new File();
	result->myImpl = new KernelFile(fileName, this, 0, 0, true, true);
	return result;
}

void KernelPartition::startReader(FileName fileName) {
	if (threadCount.find(fileName) == threadCount.end()) {
		threadCount[fileName] = 0;
		readerMutex[fileName] = CreateSemaphore(NULL, 1, 10000, NULL);
		writerMutex[fileName] = CreateSemaphore(NULL, 1, 10000, NULL);
		readerCount[fileName] = 0;
	}

	threadCount[fileName] ++;

	HANDLE resourceAccess = writerMutex[fileName];
	HANDLE readCountAccess = readerMutex[fileName];

	FS::myImpl->unlock();

	DWORD dwWaitResult = WaitForSingleObject(readCountAccess, INFINITE);
	if (readerCount[fileName] == 0) {
		DWORD dwWaitResult2 = WaitForSingleObject(resourceAccess, INFINITE);
	}
	readerCount[fileName]++;
	ReleaseSemaphore(readCountAccess, 1, NULL);

	FS::myImpl->lock();
}

void KernelPartition::startWriter(FileName fileName) {
	if (threadCount.find(fileName) == threadCount.end()) {
		threadCount[fileName] = 0;
		readerMutex[fileName] = CreateSemaphore(NULL, 1, 10000, NULL);
		writerMutex[fileName] = CreateSemaphore(NULL, 1, 10000, NULL);
		readerCount[fileName] = 0;
	}

	threadCount[fileName]++;

	HANDLE resourceAccess = writerMutex[fileName];
	HANDLE readCountAccess = readerMutex[fileName];

	FS::myImpl->unlock();

	DWORD dwWaitResult = WaitForSingleObject(resourceAccess, INFINITE);

	FS::myImpl->lock();
}


void KernelPartition::closeReaderWriter(FileName fileName, bool canWrite) {
	HANDLE resourceAccess = writerMutex[fileName];
	HANDLE readCountAccess = readerMutex[fileName];

	threadCount[fileName]--;

	if (canWrite) {
		ReleaseSemaphore(resourceAccess, 1, NULL);
	}
	else {
		DWORD dwWaitResult = WaitForSingleObject(readCountAccess, INFINITE);
		readerCount[fileName]--;
		if (readerCount[fileName] == 0) {
			ReleaseSemaphore(resourceAccess, 1, NULL);
		}
		ReleaseSemaphore(readCountAccess, 1, NULL);
	}

	if (threadCount[fileName] == 0) {
		threadCount.erase(fileName);
		CloseHandle(readerMutex[fileName]);
		readerMutex.erase(fileName);
		CloseHandle(writerMutex[fileName]);
		writerMutex.erase(fileName);
		readerCount.erase(fileName);
	}
}

bool KernelPartition::doesExist(std::string fileName) {
	auto iterator = fileEntries.find(fileName);
	return iterator != fileEntries.end();
}

char KernelPartition::deleteFile(std::string fileName) {
	auto doesExist = this->doesExist(fileName);
	if (!doesExist) {
		cout << "File " << fileName << " does not exist!" << endl;
		return '0';
	}
	auto iterator = fileEntries.find(fileName);
	if (iterator->second->size > 0) {
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
			}
			else {
				break;
			}
		}
		saveClusterToPartition(0, bitVector);
	}
	fileEntries.erase(iterator);
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

	///
	auto deepFormat = new char[2048];
	memset(deepFormat, 0, 2048);
	for (int i = 2; i < partition->getNumOfClusters(); i++) {
		partition->writeCluster(i, deepFormat);
	}
	///
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

void KernelPartition::updateIndexCluster(std::string fileName, ClusterNo indexCluster) {
	readRootDirectoryIndex();
	fileEntries[fileName]->indexCluster = indexCluster;
	writeRootDirectoryIndex();
}

void KernelPartition::updateFileSize(std::string fileName, BytesCnt size) {
	readRootDirectoryIndex();
	fileEntries[fileName]->size = size;
	writeRootDirectoryIndex();
}
