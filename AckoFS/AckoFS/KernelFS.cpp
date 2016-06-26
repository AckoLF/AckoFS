#include "KernelFS.h"

using namespace std;

KernelFS::KernelFS() {
	mutex = CreateSemaphore(NULL, 1L, 10000L, NULL);
	// check for any errors
	if (mutex == NULL) {
		auto lastError = GetLastError();
		cout << "Error! getLastError = " << hex << lastError << endl;
	}
	for (int i = 0; i < 26; i++) {
		mountedPartitions.push_back(nullptr);
	}
}

KernelFS::~KernelFS() {
}

void KernelFS::lock() {
	auto result = WaitForSingleObject(mutex, INFINITE);
	// check for any errors 
	if (result == WAIT_FAILED) {
		auto lastError = GetLastError();
		cout << "Error! getLastError = " << hex << lastError << endl;
	}
}

void KernelFS::unlock() {
	auto result = ReleaseSemaphore(mutex, 1L, 0L);
	// check for any errors
	if (result == 0) {
		auto lastError = GetLastError();
		cout << "Error! getLastError = " << hex << lastError << endl;
	}
}

char KernelFS::mount(Partition *partition) {
	for (int i = 0; i < 26; i++) {
		if (mountedPartitions[i] == nullptr) {
			mountedPartitions[i] = new KernelPartition(partition);
			return 'A' + i;		
		}
	}
	// no space to mount the partition
	return '0';
}

char KernelFS::unmount(char partitionSymbol) {
	auto partition = getPartition(partitionSymbol);
	if (partition != nullptr) {
		delete partition;
		mountedPartitions[partitionSymbol - 'A'] = nullptr;
		return '1';
	}
	else {
		return '0';
	}
}

char KernelFS::format(char partitionSymbol) {
	auto partition = getPartition(partitionSymbol);
	return partition->format();
}

File * KernelFS::open(char *fileName, char mode) {
	auto partitionSymbol = fileName[0];
	auto partition = getPartition(partitionSymbol);
	// illegal partition name or unmounted partition
	if (partition == nullptr) {
		cout << "Partition is unmounted!" << endl;
		return nullptr;
	}
	// fileName ~ A:\foo.bar
	auto fileNameString = string(fileName);
	auto filePath = fileNameString.substr(3);
	return partition->open(filePath, mode);
}

char KernelFS::readRootDir(char partitionSymbol, EntryNum entryNumber, Directory & directory) {
	auto partition = getPartition(partitionSymbol);
	// illegal partition name or unmounted partition
	if (partition == nullptr) {
		return '0';
	}
	return partition->readRootDir(entryNumber, directory);
}

char KernelFS::doesExist(char *fileName) {
	auto partitionSymbol = fileName[0];
	auto partition = getPartition(partitionSymbol);
	// illegal partition name or unmounted partition
	if (partition == nullptr) {
		return '0';
	}
	// fileName ~ A:\foo.bar
	auto fileNameString = string(fileName);
	auto filePath = fileNameString.substr(3);
	return partition->doesExist(filePath);
}

char KernelFS::deleteFile(char *fileName) {
	auto partitionSymbol = fileName[0];
	auto partition = getPartition(partitionSymbol);
	// illegal partition name or unmounted partition
	if (partition == nullptr) {
		return '0';
	}
	// fileName ~ A:\foo.bar
	auto fileNameString = string(fileName);
	auto filePath = fileNameString.substr(3);
	return partition->deleteFile(filePath);
}

KernelPartition* KernelFS::getPartition(char partitionSymbol) {
	if (('A' <= partitionSymbol) && (partitionSymbol <= 'Z')) {
		return mountedPartitions[partitionSymbol - 'A'];
	}
	else {
		return nullptr;
	}
}
