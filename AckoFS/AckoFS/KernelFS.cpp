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
			mountedPartitions[i] = make_unique<KernelPartition>(partition);
			return 'a' + i;		
		}
	}
	// no space to mount the partition
	return 'X';
}

char KernelFS::unmount(char partition) {
	// return '0' if already a nullptr?
	if (('a' <= partition) && (partition <= 'z')) {
		mountedPartitions[partition - 'a'].reset(nullptr);
		return '1';
	}
	else {
		return '0';
	}
}

char KernelFS::format(char partition) {
	cout << "format TODO!" << endl;
	return '0';
}
