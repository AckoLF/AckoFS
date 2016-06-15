#include <iostream>

#include "part.h"
#include "fs.h"

// link the partition library
#pragma comment(lib, "part.lib")

using namespace std;

int main(void) {
	cout << "Hello, my name is AckoFS! :)" << endl;
	auto p1 = new Partition("p1.ini");
	cout << "Partition p1 created!" << endl;
	auto result = FS::mount(p1);
	cout << "P1 mounted at: " << result << endl;
	result = FS::format('a');
	cout << "P1 formated: " << result << endl;
	result = FS::unmount('a');
	cout << "P1 unmonuted: " << result << endl;
	cout << "Goodbye, dear user! :(" << endl;
	return 0;
}
