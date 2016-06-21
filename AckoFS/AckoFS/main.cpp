#include <iostream>

#include "part.h"
#include "fs.h"
#include "BitVector.h"
#include "KernelCluster.h"

// link the partition library
#pragma comment(lib, "part.lib")

using namespace std;

void bitVectorTest() {
	cout << "=== bitVectorTest start ===" << endl;
	char *bitVector = new char[2048];
	memset(bitVector, 0, 2048);
	for (int i = 0; i < 2048; i++) {
		setBitValue(bitVector, i, true);
	}
	setBitValue(bitVector, 666, false);
	cout << "findFirstNotSet(666) = " << findFirstNotSet(bitVector, 2048) << endl;
	setBitValue(bitVector, 666, true);
	cout << "findFirstNotSet(NO_FREE_CLUSTERS) = " << findFirstNotSet(bitVector, 2048) << endl;
	cout << "=== bitVectorTest end ===" << endl;
}

void clusterPartitionTest() {

}

void rootDirectoryTest() {

}

int main(void) {
	bitVectorTest();

	cout << "Hello, my name is AckoFS! :)" << endl;
	auto p1 = new Partition("p1.ini");

	cout << "Partition p1 created!" << endl;
	auto result = FS::mount(p1);
	cout << "P1 mounted at: " << result << endl;
	//result = FS::format('A');
	cout << "P1 formated: " << result << endl;
	
	return 0;
	/*
	KernelCluster kernelCluster(bitVector);
	Entry entry;
	// find a way to copy string as bytes, not as string, it adds '\0' at the end which you don't need
	memcpy(entry.name, "ivanovic", 8); //c
	memcpy(entry.ext, "coa", 3); //a, hehe
	entry.reserved = 0;
	entry.indexCluster = 42L;
	entry.size = 666L;

	Entry secondEntry;
	secondEntry.splitRelativePath("vinjak.xyz");
	secondEntry.reserved = 0;
	secondEntry.indexCluster = 666L;
	secondEntry.size = 42L;

	//kernelCluster.writeClusterEntry(entry);
	//kernelCluster.seek(0);

	//auto entry2 = kernelCluster.readClusterEntry();

	//puts(entry2.name);
	//puts(entry2.ext);

	cout <<	entry.getRelativePath() << endl;

	// let's make a file yo!


	// .................................. ludi vukovi
	auto rootDirectoryIndex = new char[2048];
	memset(rootDirectoryIndex, 0, 2048);

	KernelCluster rootDirectoryCluster(rootDirectoryIndex);
	for (int i = 0; i < 2048; i++) {
		rootDirectoryCluster.writeClusterEntry(entry);
	}
	rootDirectoryCluster.writeNumber(0);
	rootDirectoryCluster.writeNumber(2);

	p1->writeCluster(1, rootDirectoryIndex);

	auto secondLevelShit = new char[2048];
	memset(secondLevelShit, 0, 2048);
	KernelCluster secondLevelCluster(secondLevelShit);
	secondLevelCluster.writeClusterEntry(secondEntry);

	p1->writeCluster(2, secondLevelShit);

	// .................................. like i ceste sinovi
	FS::open("A:\\acko.txt", 'w');
	FS::open("A:\\nijeacko.txt", 'r');
	Directory d;
	cout << "yo! readRootDir bro!" << endl;
	FS::readRootDir('A', 5, d);

	cout << "doesExist(true): " << FS::doesExist("A:\\acko.txt") << endl;
	result = FS::unmount('A');
	cout << "P1 unmonuted: " << result << endl;
	cout << "doesExist(false): " << FS::doesExist("A:\\acko.txt") << endl;
	cout << "should not open: " << FS::open("A:\\wtf.xyz", 'w') << endl;
	cout << "Goodbye, dear user! :(" << endl;
	*/
	return 0;
}
