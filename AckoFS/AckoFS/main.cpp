#include <iostream>

#include "part.h"
#include "fs.h"
#include "file.h"
#include "BitVector.h"
#include "KernelCluster.h"

// link the partition library
#pragma comment(lib, "part.lib")

using namespace std;

void bitVectorTest() {
	cout << "===== bitVectorTest start =====" << endl;
	char *bitVector = new char[2048];
	memset(bitVector, 0, 2048);
	for (int i = 0; i < 2048; i++) {
		setBitValue(bitVector, i, true);
	}
	setBitValue(bitVector, 666, false);
	cout << "findFirstNotSet(666) = " << findFirstNotSet(bitVector, 2048) << endl;
	setBitValue(bitVector, 666, true);
	cout << "findFirstNotSet(NO_FREE_CLUSTERS) = " << findFirstNotSet(bitVector, 2048) << endl;
	cout << "===== bitVectorTest end   =====" << endl;
}

void clusterPartitionTest() {

}

void rootDirectoryTest() {

}

int main(void) {
	// bitVectorTest();

	cout << "Hello, my name is AckoFS! :)" << endl;
	auto p1 = new Partition("p1.ini");
	auto p1Symbol = FS::mount(p1);
	FS::format(p1Symbol);
	cout << "P1 mounted at: " << p1Symbol << endl;
	cout << "P1 formatted!" << endl;
	char path1[42] = "1:\\fighters.foo";
	path1[0] = p1Symbol;

	char path2[42] = "1:\\vinjak.xyz";
	path2[0] = p1Symbol;

	char path3[42] = "1:\\prodigy.wtf";
	path3[0] = p1Symbol;

	char path4[42] = "1:\\hooli.xyz";
	path4[0] = p1Symbol;

	auto file1 = FS::open(path1, 'w');
	auto file2 = FS::open(path2, 'w');
	auto file3 = FS::open(path3, 'w');

	FS::deleteFile(path2);

	auto file4 = FS::open(path4, 'w');
	
	FS::deleteFile(path3);

	auto file5 = FS::open(path1, 'r');
	auto file6 = FS::open(path2, 'r');
	auto file7 = FS::open(path3, 'r');

	Entry directory[64];
	auto count = static_cast<int>(FS::readRootDir(p1Symbol, 0, directory));
	
	cout << "Found " << count << " files" << endl;
	for (auto i = 0; i < count; i++) {
		auto entry = directory[i];
		cout << entry.toString() << endl;
	}

	cout << "doesExist (hooli.xyz) = " << FS::doesExist("A:\\hooli.xyz") << endl;
	
	cout << "Some file testing stuffZ..." << endl;
	
	file1->write(4, "acko");
	FS::deleteFile(path1);
	cout << "obrisano" << endl;
	file4->write(4, "acko");

	cout << "Ooops I did it again!" << endl;
	count = static_cast<int>(FS::readRootDir(p1Symbol, 0, directory));

	cout << "Found " << count << " files" << endl;
	for (auto i = 0; i < count; i++) {
		auto entry = directory[i];
		cout << entry.toString() << endl;
	}

	return 0;
}
