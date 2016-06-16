#include <iostream>

#include "part.h"
#include "fs.h"
#include "BitVector.h"

// link the partition library
#pragma comment(lib, "part.lib")

using namespace std;

int main(void) {
	cout << "Hello, my name is AckoFS! :)" << endl;
	auto p1 = new Partition("p1.ini");
	char * buffer1 = "acko", *buffer2 = new char[2048];
	p1->writeCluster(5, buffer1);
	p1->readCluster(5, buffer2);
	puts(buffer1);
	puts(buffer2);
	cout << "Partition p1 created!" << endl;
	auto result = FS::mount(p1);
	cout << "P1 mounted at: " << result << endl;
	result = FS::format('a');
	cout << "P1 formated: " << result << endl;
	result = FS::unmount('a');
	cout << "P1 unmonuted: " << result << endl;
	cout << "Goodbye, dear user! :(" << endl;

	// bitVector testing...
	char *bitVector = new char[2048];
	memset(bitVector, 0, sizeof(bitVector));
	cout << bitVectorIdx(10) << endl;
	cout << bitVectorPosition(10) << endl;
	cout << bitVectorPosition(8) << " " << bitVectorPosition(15) << endl;
	setBitValue(bitVector, 0, true);
	setBitValue(bitVector, 7, true);
	setBitValue(bitVector, 0, false);
	unsigned char test1 = bitVector[0];
	int test2 = test1;
	cout << test2 << endl;

	return 0;
}
