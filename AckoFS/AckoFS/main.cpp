#include <iostream>

#include "part.h"
#include "fs.h"
#include "BitVector.h"
#include "KernelCluster.h"

// link the partition library
#pragma comment(lib, "part.lib")

using namespace std;

int main(void) {
	cout << "Hello, my name is AckoFS! :)" << endl;
	auto p1 = new Partition("p1.ini");
	char * buffer1 = "acko", *buffer2 = new char[2048];
	//p1->writeCluster(5, buffer1);
	//p1->readCluster(5, buffer2);
	puts(buffer1);
	puts(buffer2);
	cout << "Partition p1 created!" << endl;
	auto result = FS::mount(p1);
	cout << "P1 mounted at: " << result << endl;
	result = FS::format('A');
	cout << "P1 formated: " << result << endl;
	
	// bitVector testing...
	char *bitVector = new char[2048];
	memset(bitVector, 0, 2048);
	cout << "bitVectorIdx(10) = " << bitVectorIdx(10) << endl;
	cout << "bitVectorPos(10) = " << bitVectorPos(10) << endl;
	cout << "bitVectorPos(8) = " << bitVectorPos(8) << endl;
	cout << "bitVectorPos(15) = " << bitVectorPos(15) << endl;
	setBitValue(bitVector, 0, true);
	setBitValue(bitVector, 7, true);
	setBitValue(bitVector, 0, false);
	setBitValue(bitVector, 0, true);
	auto bitVector0 = static_cast<int>(static_cast<unsigned char>(bitVector[0]));
	cout << "bitVector0 = " << bitVector0 << endl;
	cout << "findFirstNotSet = " << findFirstNotSet(bitVector, 2048) << endl;

	p1->readCluster(0, bitVector);


	for (int i = 0; i < 10; i++) {
		printf("%d ", static_cast<int>(static_cast<unsigned char>(bitVector[i])));
	}
	printf("\n");


	cout << "findFirstNotSet from partition = " << findFirstNotSet(bitVector, 2048) << endl;

	KernelCluster kernelCluster(bitVector);
	Entry entry;
	// find a way to copy string as bytes, not as string, it adds '\0' at the end which you don't need
	memcpy(entry.name, "ivanovic", 8); //c
	memcpy(entry.ext, "coa", 3); //a, hehe
	entry.reserved = 0;
	entry.indexCluster = 42L;
	entry.size = 666L;

	//kernelCluster.writeClusterEntry(entry);
	//kernelCluster.seek(0);

	//auto entry2 = kernelCluster.readClusterEntry();

	//puts(entry2.name);
	//puts(entry2.ext);

	cout <<	entry.getRelativePath() << endl;

	// let's make a file yo!

	FS::open("A:\\acko.txt", 'w');
	FS::open("A:\\nijeacko.txt", 'r');
	Directory d;
	FS::readRootDir('A', 5, d);

	cout << FS::doesExist("A:\\acko.txt") << endl;
	result = FS::unmount('A');
	cout << "P1 unmonuted: " << result << endl;
	cout << FS::doesExist("A:\\acko.txt") << endl;
	FS::open("A:\\wtf.xyz", 'w');
	cout << "Goodbye, dear user! :(" << endl;

	return 0;
}
